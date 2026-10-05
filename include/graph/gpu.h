/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GPU_H
#define _GRAPH__GPU_H

#include <sys/gpu.h>

#include <graph/geom.h>

#include <stdint.h>

struct graph_texture_info {
	uint8_t u, v;
	uint16_t w, h;
	uint16_t page, clut;
};
typedef struct graph_texture_info graph_texture_info_t;

struct graph_texture_data {
	void *img;
	uint32_t x, y, w, h;

	void *pal;
	uint32_t px, py;
	gp0_color_depth_t depth;
};
typedef struct graph_texture_data graph_texture_data_t;

void graph_await_dma(void);
void graph_await_cmd(void);
void graph_await_vsync(void);

void graph_gpu_init(gp1_hres_t hres, gp1_vres_t vres, int width, int height);
void graph_vram_read(void *data, graph_rect_t *box);
void graph_vram_write(const void *data, graph_rect_t *box);

#endif /* _GRAPH__GPU_H */
