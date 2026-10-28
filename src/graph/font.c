/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/font.h>
#include <graph/dmadraw.h>

#include <stdbool.h>
#include <stdint.h>

void
graph_print_str(
	graph_dma_chain_t *chain,
	graph_font_t *font,
	int x,
	int y,
	const char *str
	)
{
	uint32_t
		offset_x    = 0,
		offset_y    = 0,
		tab_width   = font->cw * GRAPH_FONT_TAB_CHARS,
		space_width = font->cw,
		line_height = font->ch;

	uint32_t *ptr = graph_alloc_chain_packet(chain, 1);
	ptr[0] = gp0_set_page(font->info.page, false, false);

	uint8_t c;
	while ((c = (uint8_t)*str++) != '\0') {
		switch (c) {
		case '\t':
			offset_x += tab_width;
			offset_x -= offset_x % tab_width;
			continue;

		case '\n':
			offset_y += line_height;
		case '\r':
			offset_x = 0;
			continue;

		case ' ':
			offset_x += space_width;
			continue;
		}

		if (c < GRAPH_FONT_FIRST_CHAR || c > GRAPH_FONT_LAST_CHAR) {
			c = GRAPH_FONT_LAST_CHAR; /* DEL = invalid character */
		}

		uint32_t idx = c - GRAPH_FONT_FIRST_CHAR;
		uint32_t cx = idx / font->cols * font->cw;
		uint32_t cy = idx % font->cols * font->ch;

		graph_texture_info_t char_tex = {
			.u    = font->info.u + cx,
			.v    = font->info.v + cy,
			.w    = font->cw,
			.h    = font->ch,
			.page = font->info.page,
			.clut = font->info.clut
		};

		graph_m_sprite(
			chain,
			&GRAPH_POINT(x + offset_x, y + offset_y),
			&char_tex,
			true, true
		);

		offset_x += font->cw;
	}
}
