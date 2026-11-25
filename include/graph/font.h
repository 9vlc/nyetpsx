/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__FONT_H
#define _GRAPH__FONT_H

#include <graph/gpu.h>

#include <stdint.h>

#define GRAPH_FONT_FIRST_CHAR	32  /* ' ' */
#define GRAPH_FONT_LAST_CHAR	127 /* DEL */
#define GRAPH_FONT_TAB_WIDTH	8

struct graph_font {
	struct graph_texture_data data;
	struct graph_texture_info info;

	uint8_t cw, ch;
	uint8_t rows, cols;
};
typedef struct graph_font graph_font_t;

void
graph_load_font(
	graph_font_t *font
	);

#ifdef _GRAPH__DIRECTDRAW_H
void
graph_d_print_str(
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	);
#endif

#ifdef _GRAPH__DMADRAW_H
void
graph_m_print_str(
	graph_dma_chain_t *chain,
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	);
#endif

#ifdef _GRAPH__DMADRAW_ORD_H
void
graph_o_print_str(
	graph_dma_ord_chain_t *chain,
	uint32_t z,
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	);
#endif

#endif /* _GRAPH__FONT_H */
