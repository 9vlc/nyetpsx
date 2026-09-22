/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/freq.h>
#include <sys/irq.h>
#include <sys/timer.h>
#include <sys/sio.h>
#include <sys/gpu.h>

#include <graph/geom.h>

#include <crc32.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define SCREEN_HRES	GP1_HRES_320
#define SCREEN_VRES	GP1_VRES_256
#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240

static void
gpu_init(
	gp1_hres_t hres,
	gp1_vres_t vres,
	uint32_t width,
	uint32_t height
	)
{
	bool is_pal = ((GPU_GP1 & GP1_STAT_FB_MODE_BITMASK) == GP1_STAT_FB_MODE_PAL);

	int x = 0x760;
	int y = (is_pal ? 0xA3 : 0x88);

	int off_x = (width * gp1_clock_mult_h(hres)) / 2;
	int off_y = (height * gp1_clock_div_v(vres)) / 2;

	GPU_GP1 = gp1_reset_gpu();
	GPU_GP1 = gp1_fb_range_h(x - off_x, x + off_x);
	GPU_GP1 = gp1_fb_range_v(y - off_y, y + off_y);

	GPU_GP1 = gp1_fb_mode(
		hres,
		vres,
		(is_pal ? GP1_MODE_PAL : GP1_MODE_NTSC),
		GP1_COLOR_16BPP,
		(vres == GP1_VRES_512) /* Interlace on hires */
	);

	GPU_GP1 = gp1_disp_blank(false);
}

int
main(void)
{
	gpu_init(
		SCREEN_HRES,
		SCREEN_VRES,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);

	/* Set page and screen origin */
	graph_await_cmd();
	GPU_GP0 = gp0_set_page(0, true, false);
	GPU_GP0 = gp0_fb_offset1(0, 0);
	GPU_GP0 = gp0_fb_offset2(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	GPU_GP0 = gp0_fb_origin(0, 0);

	/*
	 * Breathing animation thingy
	 */
	uint8_t rcc = 0;
	int inc = 1;
	while (1) {
		graph_d_rect(
			GRAPH_RECT(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
			GRAPH_XRGB(0x000000),
			false
		);

		/* Triangle on left side */
		graph_d_shad_tri(
			GRAPH_TRI(
				0, 0,
				0, SCREEN_HEIGHT,
				SCREEN_WIDTH, SCREEN_HEIGHT
			),
			GRAPH_XRGB(0x000000),
			GRAPH_XRGB(rcc << 16),
			GRAPH_XRGB(0x000000),
			false
		);

		/* Triangle on right side */
		graph_d_shad_tri(
			GRAPH_TRI(
				0, 0,
				SCREEN_WIDTH, 0,
				SCREEN_WIDTH, SCREEN_HEIGHT
			),
			GRAPH_XRGB(0x000000),
			GRAPH_XRGB(rcc << 16),
			GRAPH_XRGB(0x000000),
			false
		);

		graph_await_cmd();
		graph_await_vsync();

		rcc += inc;
		if (rcc >= 200) {
			inc = -1;
		} else if (rcc <= 100) {
			inc = 1;
		}
	}
}
