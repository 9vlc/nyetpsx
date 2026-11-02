/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GPU_H
#define _GRAPH__GPU_H

#define PACKED __attribute__((packed))

#include <psx/gpu.h>

#include <graph/geom.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define GRAPH_DMA_MAX_CHUNK_SIZE	16
#define GRAPH_DMA_CHAIN_BUF_SIZE	4096
#define GRAPH_DMA_CHAIN_OT_SIZE		240

#define GRAPH_VRAM_X_ALIGN_UP(X)	((X) + 16 - ((X) % 16))
#define GRAPH_VRAM_X_ALIGN_DOWN(X)	((X) -      ((X) % 16))

/*
 * Pixel formats
 */

union graph_rgb24 {
	struct { uint8_t r, g, b; } c;
	uint32_t raw;
} PACKED;
typedef union graph_rgb24 graph_rgb24_t;
#define GRAPH_RGB24(R, G, B) (graph_rgb24_t){.c.r=(R), .c.g=(G), .c.b=(B)}
#define GRAPH_XRGB24(X) (graph_rgb24_t){.raw=gp0_xrgb24(X)}

union graph_rgb16 {
	struct { uint16_t r:5, g:5, b:5, stp:1; } c;
	uint16_t raw;
} PACKED;
typedef union graph_rgb16 graph_rgb16_t;
#define GRAPH_RGB16(R, G, B) (graph_rgb16_t){.c.r=(R), .c.g=(G), .c.b=(B), .c.stp=0}
#define GRAPH_XRGB16(X) (graph_rgb16_t){.raw=gp0_xrgb16((X), false)}
#define GRAPH_RGBA16(R, G, B, S) (graph_rgb16_t){.c.r=(R), .c.g=(G), .c.b=(B), .c.stp=(S)}
#define GRAPH_XRGBA16(X, S) (graph_rgb16_t){.raw=gp0_xrgb16((X), (S))}

/*
 * Chains
 */

struct graph_dma_chain {
	uint32_t data[GRAPH_DMA_CHAIN_BUF_SIZE];
	uint32_t *next;
};
typedef struct graph_dma_chain graph_dma_chain_t;

struct graph_dma_ord_chain {
	uint32_t data[GRAPH_DMA_CHAIN_BUF_SIZE];
	uint32_t ot[GRAPH_DMA_CHAIN_OT_SIZE];
	uint32_t *next;
};
typedef struct graph_dma_ord_chain graph_dma_ord_chain_t;

/*
 * Textures
 */

/* Texture in ROM / RAM */
struct graph_texture_data {
	const void *img;
	uint32_t x, y, w, h;

	const void *clut;
	uint32_t cx, cy;
	gp0_color_depth_t depth;
};
typedef struct graph_texture_data graph_texture_data_t;

/* Texture in VRAM */
struct graph_texture_info {
	uint8_t u, v;
	uint16_t w, h;
	uint16_t page, clut;
};
typedef struct graph_texture_info graph_texture_info_t;

/* ... */

void
graph_init_gpu(
	gp1_hres_t hres,
	gp1_vres_t vres,
	int width,
	int height
	);

void
graph_read_vram(
	void *data,
	graph_rect_t *box
	);

void
graph_write_vram(
	const void *data,
	graph_rect_t *box
	);

void
graph_load_texture(
	graph_texture_data_t *data,
	graph_texture_info_t *info
	);

uint32_t *
graph_alloc_chain_packet(
	graph_dma_chain_t *chain,
	size_t cmds
	);

uint32_t *
graph_alloc_ord_chain_packet(
	graph_dma_ord_chain_t *chain,
	uint32_t z,
	size_t cmds
	);

void
graph_send_chain(
	const uint32_t *data
	);

void
graph_clear_ot(
	uint32_t *ot,
	size_t size
	);

#undef PACKED

#endif /* _GRAPH__GPU_H */
