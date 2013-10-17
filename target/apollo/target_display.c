/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
#include <smem.h>
#include <msm_panel.h>
#include <pm8x41.h>
#include <pm8x41_wled.h>
#include <board.h>
#include <mdp5.h>
#include <platform/gpio.h>
#include <platform/iomap.h>
#include <platform/clock.h>
#include <target/display.h>
#include <dev/fbgfx.h>

#define HEIGHT_IMAGE_CHARGE  219
#define Y_POSITION_LOW_POWER  1269 /* for  y */
#define Y_POSITION_CHARGER  (Y_POSITION_LOW_POWER -  HEIGHT_IMAGE_CHARGE - 78)  /* for  y */
#define Y_POSITION_LOGO  1343 /* for  y */
#define Y_POSITION_HOT  1162 /* for  y */

extern struct fbgfx_image image_charge;
extern struct fbgfx_image image_hot;
extern struct fbgfx_image image_low;
extern struct fbgfx_image image_boot_Kindle;

static struct msm_fb_panel_data panel;
static uint8_t display_enable;
static int image_type = 0;

extern int msm_display_init(struct msm_fb_panel_data *pdata);
extern int msm_display_off();
extern int apollo_dsi_uniphy_pll_config(uint32_t ctl_base);

static int apollo_mdss_dsi_panel_clock(uint8_t enable)
{
	uint32_t dual_dsi = panel.panel_info.mipi.dual_dsi;
	if (enable) {
		mdp_gdsc_ctrl(enable);
		mdp_clock_init();
		apollo_dsi_uniphy_pll_config(MIPI_DSI0_BASE);
		mmss_clock_init(DSI0_PHY_PLL_OUT, dual_dsi);
	} else if(!target_cont_splash_screen()) {
		// * Add here for continuous splash  *
	}

	return 0;
}

/* Pull DISP_RST_N high to get panel out of reset */
static void apollo_mdss_mipi_panel_reset(uint8_t enable)
{
	struct pm8x41_gpio gpio19_param = {
		.direction = PM_GPIO_DIR_OUT,
		.output_buffer = PM_GPIO_OUT_CMOS,
		.out_strength = PM_GPIO_OUT_DRIVE_MED,
		.vin_sel = 0x02,
	};

	struct pm8x41_gpio gpio20_param = {
		.direction = PM_GPIO_DIR_OUT,
		.output_buffer = PM_GPIO_OUT_CMOS,
		.out_strength = PM_GPIO_OUT_DRIVE_MED,
	};

	pm8x41_gpio_config(19, &gpio19_param);
	if (enable) {
		gpio_tlmm_config(58, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA, GPIO_DISABLE);
		gpio_tlmm_config(0, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_8MA, GPIO_DISABLE);
		gpio_tlmm_config(1, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE);

		pm8x41_gpio_set(19, PM_GPIO_FUNC_LOW);
		mdelay(20);
		gpio_set(1, 0);
		mdelay(20);
		gpio_set(58, 2);
		mdelay(20);
		gpio_set(0, 2);
		mdelay(15);
		pm8x41_gpio_set(19, PM_GPIO_FUNC_HIGH);
		mdelay(15);
	} else {
		gpio_tlmm_config(58, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE);
		gpio_tlmm_config(0, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE);
		gpio19_param.out_strength = PM_GPIO_OUT_DRIVE_LOW;
		gpio20_param.out_strength = PM_GPIO_OUT_DRIVE_LOW;
		pm8x41_gpio_config(19, &gpio19_param);
		pm8x41_gpio_set(19, PM_GPIO_FUNC_LOW);
		pm8x41_gpio_set(20, PM_GPIO_FUNC_LOW);
		gpio_set(58, 2);
		gpio_set(0, 0);
	}
}

static int apollo_mipi_panel_power(uint8_t enable)
{
	if (enable) {
		/* backlight enable is out of this */

		/* Turn on LDO8 for lcd1 mipi vdd */
		dprintf(SPEW, " Setting LDO22\n");
		pm8x41_ldo_set_voltage("LDO22", 3000000);
		pm8x41_ldo_control("LDO22", enable);

		dprintf(SPEW, " Setting LDO12\n");
		/* Turn on LDO23 for lcd1 mipi vddio */
		pm8x41_ldo_set_voltage("LDO12", 1800000);
		pm8x41_ldo_control("LDO12", enable);

		dprintf(SPEW, " Setting LDO2\n");
		/* Turn on LDO2 for vdda_mipi_dsi */
		pm8x41_ldo_set_voltage("LDO2", 1200000);
		pm8x41_ldo_control("LDO2", enable);

		dprintf(SPEW, " Panel Reset \n");
		/* Panel Reset */
		apollo_mdss_mipi_panel_reset(enable);
		dprintf(SPEW, " Panel Reset Done\n");
	} else {
		apollo_mdss_mipi_panel_reset(enable);
		pm8x41_ldo_control("LDO2", enable);
		pm8x41_ldo_control("LDO22", enable);

	}

	return 0;
}

void display_init(void)
{
	uint32_t hw_id = board_hardware_id();
	uint32_t soc_ver = board_soc_version();

	dprintf(INFO, "display_init(),target_id=%d.\n", hw_id);

        if (!display_enable)
        {
		mipi_jdi_video_wqxga_init(&(panel.panel_info));
		panel.clk_func = apollo_mdss_dsi_panel_clock;
		panel.power_func = apollo_mipi_panel_power;
		panel.fb.base = MIPI_FB_ADDR;
		panel.fb.width =  panel.panel_info.xres;
		panel.fb.height =  panel.panel_info.yres;
		panel.fb.stride =  panel.panel_info.xres;
		panel.fb.bpp =  panel.panel_info.bpp;
		panel.fb.format = FB_FORMAT_RGB888;
		panel.mdp_rev = MDP_REV_50;

		if (msm_display_init(&panel)) {
			dprintf(CRITICAL, "Display init failed!\n");
			return;
		}
		display_enable = 1;
        }
}

void display_shutdown(void)
{
	if (display_enable){
		if(!target_cont_splash_screen()) {
			lp855x_bl_off();
			set_display_image_type(IMAGE_NONE);
		}
                msm_display_off();
       }
}

int get_display_image_type()
{
        return image_type;
}

void set_display_image_type(Image_types type)
{
        image_type = type;
}

void display_clear(void)
{
	fbgfx_clear();
	set_display_image_type(IMAGE_NONE);
}

void show_image(Image_types type)
{
        dprintf(CRITICAL, "%s: Image_types=%d\n",__func__,type);
        display_init();
        set_display_image_type(type);
        fbgfx_clear();
        fbgfx_init();
        switch(type)
        {
            case IMAGE_CHARGING:
                fbgfx_apply_image(&image_low, Y_POSITION_LOW_POWER, FBGFX_CENTERED);
                fbgfx_apply_image(&image_charge, Y_POSITION_CHARGER, FBGFX_CENTERED);
                break;
            case IMAGE_LOWBATTERY:
                fbgfx_apply_image(&image_low, Y_POSITION_LOW_POWER, FBGFX_CENTERED);
                break;
            case IMAGE_DEVICEHOT:
                fbgfx_apply_image(&image_hot, Y_POSITION_HOT, FBGFX_CENTERED);
                break;
            case IMAGE_LOGO:
            default:
                fbgfx_apply_image(&image_boot_Kindle, Y_POSITION_LOGO, FBGFX_CENTERED);
                break;
        }
        fbgfx_flip();
}
