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
#define GRAPH_FONT_TAB_CHARS	8

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

#endif /* _GRAPH__FONT_H */
