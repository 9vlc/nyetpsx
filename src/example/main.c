/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/gte.h>
#include <graph/geom.h>
#include <graph/dmadraw.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define SCREEN_HRES	GP1_HRES_640
#define SCREEN_VRES	GP1_VRES_512
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define BLACK		GRAPH_XRGB(0)
#define WHITE		GRAPH_XRGB(0xFFFFFF)
#define RED		GRAPH_XRGB(0xFF0000)

#define BG		BLACK
#define FG		RED

#define CUBE_SIZE	1500
#define CUBE_POV_DEPTH	5000

static void
draw_clear_bg(
	graph_dma_chain_t *chain
	)
{
	graph_m_rect(
		chain,
		&GRAPH_RECT(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
		BG,
		false
	);
}

static uint8_t grad_rcc = 0;
static int32_t grad_inc = 1;
static void
draw_grad(
	graph_dma_chain_t *chain
	)
{
	/* Triangle on left side */
	graph_m_shad_tri(
		chain,
		&GRAPH_TRI(
			0, 0,
			0, SCREEN_HEIGHT,
			SCREEN_WIDTH, SCREEN_HEIGHT
		),
		BG,
		GRAPH_RGB(grad_rcc, 0, 0),
		BG,
		false
	);

	/* Triangle on right side */
	graph_m_shad_tri(
		chain,
		&GRAPH_TRI(
			0, 0,
			SCREEN_WIDTH, 0,
			SCREEN_WIDTH, SCREEN_HEIGHT
		),
		BG,
		GRAPH_RGB(grad_rcc, 0, 0),
		BG,
		false
	);

	grad_rcc += grad_inc;

	if ((grad_rcc >= 200 && grad_inc > 0) ||
		(grad_rcc <= 100 && grad_inc < 0)) {

		grad_inc *= -1;
	}
}

static const gte_vec16_t cube_verts[] = {
	{ -CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0 },
	{  CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE, 0 },
	{ -CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE, 0 },
	{  CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE, 0 },
	{ -CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE, 0 },
	{  CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE, 0 },
	{ -CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE, 0 },
	{  CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE, 0 }
};

static const uint8_t cube_edges[][2] = {
	{ 0, 1 }, { 1, 3 }, { 3, 2 }, { 2, 0 },
	{ 4, 5 }, { 5, 7 }, { 7, 6 }, { 6, 4 },
	{ 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
};

static int cube_frame = 0;
static void
draw_cube_wireframe(
	graph_dma_chain_t *chain
	)
{
	graph_point_t points[8];
	graph_line_t line;
	uint32_t sxy;

	gte_set_rot_mtx(
		GRAPH_GTE_UNIT, 0, 0,
		0, GRAPH_GTE_UNIT, 0,
		0, 0, GRAPH_GTE_UNIT
	);
	graph_rot_mtx_x(cube_frame * 12);
	graph_rot_mtx_y(cube_frame * 16);

	gte_set_ctrl_reg(GTE_TRX, 0);
	gte_set_ctrl_reg(GTE_TRY, 0);
	gte_set_ctrl_reg(GTE_TRZ, CUBE_POV_DEPTH);

	for (int i = 0; i < 8; i++) {
		gte_load_V0(&cube_verts[i]);
		gte_cmd(GTE_CMD_RTPS | GTE_SF);
		gte_load_delay();

		sxy = gte_get_data_reg(GTE_SXY2);
		points[i].x = (int16_t)(sxy & 0xFFFF);
		points[i].y = (int16_t)(sxy >> 16);
	}

	for (int i = 0; i < 12; i++) {
		line.p0 = points[cube_edges[i][0]];
		line.p1 = points[cube_edges[i][1]];

		graph_m_line(chain, &line, FG, false);
	}

	cube_frame++;
}

int
main(
	int argc,
	char **argv
	)
{
	(void)argc;
	(void)argv;

	graph_init_gpu(
		SCREEN_HRES,
		SCREEN_VRES,
		SCREEN_WIDTH,
		SCREEN_HEIGHT
	);
	graph_init_gte(SCREEN_WIDTH, SCREEN_HEIGHT);

	graph_dma_chain_t chain;
	while (1) {
		gpu_await_dma();
		chain.next = chain.data;

		draw_clear_bg(&chain);
		draw_grad(&chain);
		draw_cube_wireframe(&chain);
		graph_m_end(&chain);

		gpu_await_vsync();
		graph_send_chain(chain.data);
	}

	return (0);
}
