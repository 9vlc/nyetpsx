/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/font.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

void
graph_load_font(
	graph_font_t *font
	)
{
	graph_texture_data_t data = font->data;

	int ncol = 0, wdiv = 0;

	if (data.clut != NULL) {
		assert(data.depth != GP0_COLOR_16BPP);

		ncol = (data.depth == GP0_COLOR_8BPP ? 256 : 16);
		wdiv = (data.depth == GP0_COLOR_8BPP ? 1 : 2);
	}

	/*
	 * Load into top right corner
	 * Temporary for now, since this only allows for loading a single font
	 */
	data.x  = 1024 - (data.w >> wdiv);
	data.y  = 0;
	data.cx = ((data.x - ncol) / 16) * 16;
	data.cy = 0;

	graph_load_texture(&font->info, &data);
}
