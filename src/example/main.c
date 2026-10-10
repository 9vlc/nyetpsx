/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/gpu.h>

#include <graph/gpu.h>
#include <graph/gte.h>
#include <graph/geom.h>
#include <graph/dmadraw.h>

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

static void
draw_clear_bg(graph_dma_chain_t *chain)
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
draw_grad(graph_dma_chain_t *chain)
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

/* TODO */
static void
draw_cube_wireframe(graph_dma_chain_t *chain)
{

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
