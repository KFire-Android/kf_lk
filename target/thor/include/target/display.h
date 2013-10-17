/* Copyright (c) 2012-2013, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef _TARGET_COPPER_DISPLAY_H
#define _TARGET_COPPER_DISPLAY_H

#define MIPI_FB_ADDR  0x7F000000

#define MIPI_HSYNC_PULSE_WIDTH       24
#define MIPI_HSYNC_BACK_PORCH_DCLK   16
#define MIPI_HSYNC_FRONT_PORCH_DCLK  48

#define MIPI_VSYNC_PULSE_WIDTH       2
#define MIPI_VSYNC_BACK_PORCH_LINES  9
#define MIPI_VSYNC_FRONT_PORCH_LINES 10

typedef enum {
        IMAGE_NONE = 0,
        IMAGE_LOGO,
        IMAGE_CHARGING,
        IMAGE_LOWBATTERY,
        IMAGE_DEVICEHOT
} Image_types;

extern int mdss_dsi_phy_init(struct mipi_dsi_panel_config *, uint32_t ctl_base);
extern int thor_dsi_uniphy_pll_config(uint32_t ctl_base);

#endif
