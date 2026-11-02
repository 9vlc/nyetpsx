/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>
#include <graph/gpu.h>

#define SCREEN_HRES	GP1_HRES_640
#define SCREEN_VRES	GP1_VRES_512
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

/* Maximum drawing function Z (higher -> further away from the "camera") */
#define Z_MAX GRAPH_DMA_CHAIN_OT_SIZE - 1

/* Spinning cube */
#define CUBE_SIZE	200
#define CUBE_CAMERA_X	0
#define CUBE_CAMERA_Y	0
#define CUBE_CAMERA_Z	550

/* Mandelbrot cube texture */
#define MANDEL_SHIFT	26
#define MANDEL_UNIT	(1 << MANDEL_SHIFT)
#define MANDEL_SCALE(N)	((int32_t)(((int64_t)(N) * MANDEL_UNIT) / 10000))
#define MANDEL_MAX_ITER	50
#define MANDEL_ETIME	(3 * MANDEL_UNIT)

#define MANDEL_WIDTH	256
#define MANDEL_HEIGHT	256

#define MANDEL_LL_X	MANDEL_SCALE(-12000)
#define MANDEL_LL_Y	MANDEL_SCALE(  2000)
#define MANDEL_UR_X	MANDEL_SCALE(-10000)
#define MANDEL_UR_Y	MANDEL_SCALE ( 3500)

/* Colors */
#define COL_BG		GRAPH_XRGB24(0x505050)
#define COL_CUBE	GRAPH_XRGB24(0xA0A0A0)

void
cube_init(
	graph_texture_info_t *tex
	);

void
cube_render(
	graph_dma_ord_chain_t *chain
	);

void
mandel_render_line(
	uint8_t *fbuf,
	uint32_t width,
	uint32_t height,
	uint32_t y,
	int32_t ll_x,
	int32_t ll_y,
	int32_t ur_x,
	int32_t ur_y,
	uint32_t max_iter
	);

void
mandel_generate_clut(
	graph_rgb16_t *clut
	);
