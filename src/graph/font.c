/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <graph/gpu.h>
#include <graph/font.h>

void
graph_load_font(
	graph_font_t *font
	)
{
	graph_load_texture(&font->data, &font->info);
}
