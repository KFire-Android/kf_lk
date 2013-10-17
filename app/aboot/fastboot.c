/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <debug.h>
#include <string.h>
#include <stdlib.h>
#include <platform.h>
#include <kernel/thread.h>
#include <kernel/event.h>
#include <dev/udc.h>
#include "fastboot.h"

// ACOS_MOD_START
#if WITH_FBGFX_FASTBOOT_OUTPUT
#include <dev/fbgfx.h>
#if WITH_FBGFX_SPLASH
extern struct fbgfx_image splash;
#endif
#define set_foreground_color fbgfx_set_foreground_color
#define printf(...) fbgfx_printf(__VA_ARGS__); fbgfx_flip()
#else
#define set_foreground_color(__ignore__)
#define printf(__ignore__, ...)
#endif
// ACOS_MOD_END

#define MAX_RSP_SIZE 64
#define MAX_USBFS_BULK_SIZE (32 * 1024)

void boot_linux(void *bootimg, unsigned sz);

/* todo: give lk strtoul and nuke this */
static unsigned hex2unsigned(const char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

struct fastboot_cmd {
	struct fastboot_cmd *next;
	const char *prefix;
	unsigned prefix_len;
	void (*handle)(const char *arg, void *data, unsigned sz);
};

struct fastboot_var {
	struct fastboot_var *next;
	const char *name;
	const char *value;
};

static struct fastboot_cmd *cmdlist;

void fastboot_register(const char *prefix,
		       void (*handle)(const char *arg, void *data, unsigned sz))
{
	struct fastboot_cmd *cmd;
	cmd = malloc(sizeof(*cmd));
	if (cmd) {
		cmd->prefix = prefix;
		cmd->prefix_len = strlen(prefix);
		cmd->handle = handle;
		cmd->next = cmdlist;
		cmdlist = cmd;
	}
}

static struct fastboot_var *varlist;

void fastboot_publish(const char *name, const char *value)
{
	struct fastboot_var *var;
	var = malloc(sizeof(*var));
	if (var) {
		var->name = name;
		var->value = value;
		var->next = varlist;
		varlist = var;
	}
}


static event_t usb_online;
static event_t txn_done;
static struct udc_endpoint *in, *out;
static struct udc_request *req;
int txn_status;

static void *download_base;
static unsigned download_max;
static unsigned download_size;

#define STATE_OFFLINE	0
#define STATE_COMMAND	1
#define STATE_COMPLETE	2
#define STATE_ERROR	3

static unsigned fastboot_state = STATE_OFFLINE;

static void req_complete(struct udc_request *req, unsigned actual, int status)
{
	txn_status = status;
	req->length = actual;

	event_signal(&txn_done, 0);
}

static int usb_read(void *_buf, unsigned len)
{
	int r;
	unsigned xfer;
	unsigned char *buf = _buf;
	int count = 0;

	if (fastboot_state == STATE_ERROR)
		goto oops;

	while (len > 0) {
		xfer = (len > MAX_USBFS_BULK_SIZE) ? MAX_USBFS_BULK_SIZE : len;
		req->buf = PA((addr_t)buf);
		req->length = xfer;
		req->complete = req_complete;
		r = udc_request_queue(out, req);
		if (r < 0) {
			dprintf(INFO, "usb_read() queue failed\n");
			goto oops;
		}
		event_wait(&txn_done);

		if (txn_status < 0) {
			dprintf(INFO, "usb_read() transaction failed\n");
			goto oops;
		}

		count += req->length;
		buf += req->length;
		len -= req->length;

		/* short transfer? */
		if (req->length != xfer) break;
	}
	/*
	 * Force reload of buffer from memory
	 * since transaction is complete now.
	 */
	arch_invalidate_cache_range(_buf, count);
	return count;

oops:
	fastboot_state = STATE_ERROR;
	return -1;
}

int usb_write(void *buf, unsigned len)
{
	int r;

	if (fastboot_state == STATE_ERROR)
		goto oops;

	req->buf = PA((addr_t)buf);
	req->length = len;
	req->complete = req_complete;
	r = udc_request_queue(in, req);
	if (r < 0) {
		dprintf(INFO, "usb_write() queue failed\n");
		goto oops;
	}
	event_wait(&txn_done);
	if (txn_status < 0) {
		dprintf(INFO, "usb_write() transaction failed\n");
		goto oops;
	}
	return req->length;

oops:
	fastboot_state = STATE_ERROR;
	return -1;
}

void fastboot_ack(const char *code, const char *reason)
{
	STACKBUF_DMA_ALIGN(response, MAX_RSP_SIZE);

	if (fastboot_state != STATE_COMMAND)
		return;

	if (reason == 0)
		reason = "";

	snprintf(response, MAX_RSP_SIZE, "%s%s", code, reason);
	fastboot_state = STATE_COMPLETE;

	usb_write(response, strlen(response));

}

void fastboot_info(const char *reason)
{
	STACKBUF_DMA_ALIGN(response, MAX_RSP_SIZE);

	if (fastboot_state != STATE_COMMAND)
		return;

	if (reason == 0)
		return;

	snprintf(response, MAX_RSP_SIZE, "INFO%s", reason);

	usb_write(response, strlen(response));
// ACOS_MOD_START
	set_foreground_color(0xffffff);
	printf("%s\n", reason);
// ACOS_MOD_END
}

void fastboot_fail(const char *reason)
{
	fastboot_ack("FAIL", reason);
// ACOS_MOD_START
	set_foreground_color(0xff0000);
	printf("FAIL: %s.\n", reason ? reason : "<no reason given>");
// ACOS_MOD_END
}

void fastboot_okay(const char *info)
{
	fastboot_ack("OKAY", info);
// ACOS_MOD_START
	set_foreground_color(0x00ff00);
	if (info != NULL && *info != 0) {
		set_foreground_color(0x808080);
		printf("[[ ");
		set_foreground_color(0x00ff00);
		printf("%s", info);
		set_foreground_color(0x808080);
		printf(" ]].\n");
	} else {
		printf("Ok.\n");
	}
// ACOS_MOD_END
}

static void cmd_getvar(const char *arg, void *data, unsigned sz)
{
	struct fastboot_var *var;

	for (var = varlist; var; var = var->next) {
		if (!strcmp(var->name, arg)) {
			fastboot_okay(var->value);
			return;
		}
	}
	fastboot_okay("");
}

static void cmd_download(const char *arg, void *data, unsigned sz)
{
	STACKBUF_DMA_ALIGN(response, MAX_RSP_SIZE);
	unsigned len = hex2unsigned(arg);
	int r;

	download_size = 0;
	if (len > download_max) {
		fastboot_fail("data too large");
		return;
	}

	snprintf(response, MAX_RSP_SIZE, "DATA%08x", len);
	if (usb_write(response, strlen(response)) < 0)
		return;

	r = usb_read(download_base, len);
	if ((r < 0) || ((unsigned) r != len)) {
		fastboot_state = STATE_ERROR;
		return;
	}
	download_size = len;
	fastboot_okay("");
}

static void fastboot_command_loop(void)
{
	struct fastboot_cmd *cmd;
	int r;
	dprintf(INFO,"fastboot: processing commands\n");

	uint8_t *buffer = (uint8_t *)memalign(CACHE_LINE, ROUNDUP(4096, CACHE_LINE));
	if (!buffer)
	{
		dprintf(CRITICAL, "Could not allocate memory for fastboot buffer\n.");
		ASSERT(0);
	}
again:
	while (fastboot_state != STATE_ERROR) {
// ACOS_MOD_START
		set_foreground_color(0xffffff);
		printf("Ready.   \r");
// ACOS_MOD_END
		r = usb_read(buffer, MAX_RSP_SIZE);
		if (r < 0) break;
		buffer[r] = 0;
		dprintf(INFO,"fastboot: %s\n", buffer);
// ACOS_MOD_START
		set_foreground_color(0xffffff);
		printf("%s...", buffer);
// ACOS_MOD_END

		for (cmd = cmdlist; cmd; cmd = cmd->next) {
			if (memcmp(buffer, cmd->prefix, cmd->prefix_len))
				continue;
			fastboot_state = STATE_COMMAND;
			cmd->handle((const char*) buffer + cmd->prefix_len,
				    (void*) download_base, download_size);
			if (fastboot_state == STATE_COMMAND)
				fastboot_fail("unknown reason");
			goto again;
		}

		fastboot_fail("unknown command");

	}
	fastboot_state = STATE_OFFLINE;
	dprintf(INFO,"fastboot: oops!\n");
// ACOS_MOD_START
	set_foreground_color(0x808080);
	printf("[OFFLINE]\r");
// ACOS_MOD_END
	free(buffer);
}

static int fastboot_handler(void *arg)
{
	for (;;) {
		event_wait(&usb_online);
		fastboot_command_loop();
	}
	return 0;
}

static void fastboot_notify(struct udc_gadget *gadget, unsigned event)
{
	if (event == UDC_EVENT_ONLINE) {
		event_signal(&usb_online, 0);
	}
}

static struct udc_endpoint *fastboot_endpoints[2];

static struct udc_gadget fastboot_gadget = {
	.notify		= fastboot_notify,
	.ifc_class	= 0xff,
	.ifc_subclass	= 0x42,
	.ifc_protocol	= 0x03,
	.ifc_endpoints	= 2,
	.ifc_string	= "fastboot",
	.ept		= fastboot_endpoints,
};

int fastboot_init(void *base, unsigned size)
{
//ACOS_MOD_START
#if (WITH_FBGFX_FASTBOOT_OUTPUT && WITH_FBGFX_SPLASH)
	unsigned int display_width = 0, display_height = 0;
	unsigned int font_width = 0, font_height = 0;
	unsigned int scroll_height;
	unsigned int scroll_step;
	unsigned int image_bottom;
#endif
//ACOS_MOD_END
	thread_t *thr;
	dprintf(INFO, "fastboot_init()\n");

//ACOS_MOD_START
#if WITH_FBGFX_FASTBOOT_OUTPUT
#if WITH_FBGFX_SPLASH
	/*
	 * scroll the splash image up, to be centered on the top quarter line of the screen;
	 * emit "[  fastboot  ]" in gray at the bottom right of the image;
	 * establish console output below the image and fastboot text, to emit fastboot output.
	 */
	fbgfx_get_display_dimensions(&display_width, &display_height);
	fbgfx_get_font_dimensions(NULL, &font_width, &font_height);
	if (display_width != 0 && display_height != 0 && font_width != 0 && font_height != 0) {
		/*
		 * scroll up by 1/4 of the display height, since the splash was already
		 * centered on the midpoint. if the image is too tall, just scroll up
		 * enough to get it flush with the top of the screen.
		 */
		scroll_height = display_height / 2;
		if (splash.height <= scroll_height) {
			scroll_height /= 2;
		} else {
			scroll_height -= (splash.height + 1) / 2;
		}
		image_bottom = (display_height / 2) + (splash.height / 2) - scroll_height;

		/* make sure there are 20 iterations of the loop regardless of size */
		scroll_step = (scroll_height + 79) / 20;
		if (scroll_step == 0) {
			scroll_step = 1;
		}
		for (;
		     scroll_height > 0;
		     scroll_height -= scroll_step) {

			if (scroll_step > scroll_height || scroll_step == 0) {
				scroll_step = scroll_height;
			}
			fbgfx_scroll_up(scroll_step, 0, 0, FBGFX_MAX, FBGFX_MAX);
			fbgfx_flip();
		}

		/*
		 * if the splash is small, just center fastboot text; otherwise place
		 * it near the lower right edge of the image
		 */
		font_width *= 14; /* number of chars in fastboot text */
		if (splash.width < font_width + 100) {
			fbgfx_set_text_window(FBGFX_CENTERED, image_bottom, font_width, FBGFX_MAX);
		} else {
			fbgfx_set_text_window((display_width / 2) + (splash.width / 2) - font_width - 50, image_bottom, FBGFX_MAX, FBGFX_MAX);
		}
		fbgfx_set_foreground_color(0x808080);
		fbgfx_printf("[  fastboot  ]");
		fbgfx_flip();

		fbgfx_set_text_window(50, image_bottom + (font_height * 2), FBGFX_MAX, FBGFX_MAX);
	}
#endif // WITH_FBGFX_SPLASH
	fbgfx_set_foreground_color(0xffffff);
#endif // WITH_FBGFX_FASTBOOT_OUTPUT
//ACOS_MOD_END

	download_base = base;
	download_max = size;

	event_init(&usb_online, 0, EVENT_FLAG_AUTOUNSIGNAL);
	event_init(&txn_done, 0, EVENT_FLAG_AUTOUNSIGNAL);

	in = udc_endpoint_alloc(UDC_TYPE_BULK_IN, 512);
	if (!in)
		goto fail_alloc_in;
	out = udc_endpoint_alloc(UDC_TYPE_BULK_OUT, 512);
	if (!out)
		goto fail_alloc_out;

	fastboot_endpoints[0] = in;
	fastboot_endpoints[1] = out;

	req = udc_request_alloc();
	if (!req)
		goto fail_alloc_req;

	if (udc_register_gadget(&fastboot_gadget))
		goto fail_udc_register;

	fastboot_register("getvar:", cmd_getvar);
	fastboot_register("download:", cmd_download);
	fastboot_publish("version", "0.5");

	thr = thread_create("fastboot", fastboot_handler, 0, DEFAULT_PRIORITY, 4096);
	if (!thr)
	{
		goto fail_alloc_in;
	}
	thread_resume(thr);
	return 0;

fail_udc_register:
	udc_request_free(req);
fail_alloc_req:
	udc_endpoint_free(out);
fail_alloc_out:
	udc_endpoint_free(in);
fail_alloc_in:
	return -1;
}
