/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <graph/gpu.h>
#include <graph/geom.h>

#include <graph/directdraw.h>
#include <graph/dmadraw.h>
#include <graph/dmadraw_ord.h>

#include <graph/font.h>

struct graph__m_emit_ctx {
	graph_dma_chain_t *chain;
};
typedef struct graph__m_emit_ctx graph__m_emit_ctx_t;

struct graph__o_emit_ctx {
	graph_dma_ord_chain_t *chain;
	uint32_t z;
};
typedef struct graph__o_emit_ctx graph__o_emit_ctx_t;

void
graph_load_font(
	graph_font_t *font
	)
{
	graph_load_texture(&font->data, &font->info);
}

/* CLUT fun */
void
graph_load_font_inverted(
	graph_font_t *font,
	bool strip_stp
	)
{
	graph_rgb16_t iclut[16];
	memcpy(iclut, font->data.clut, sizeof(iclut));
	graph_texture_data_t idata = font->data;
	idata.clut = iclut;

	for (int i = 0; i < 16; i++) {
		if (iclut[i].raw == 0x0000) {
			continue;
		} else if (strip_stp && iclut[i].raw & 0x8000) {
			iclut[i].raw = 0x0000;
			continue;
		}

		/* May want to replace this with just a plain b/w invert? */
		iclut[i].c.r = 31 - iclut[i].c.r;
		iclut[i].c.g = 31 - iclut[i].c.g;
		iclut[i].c.b = 31 - iclut[i].c.b;

		if ((iclut[i].raw & 0x7FFF) == 0) {
			iclut[i].c.r = 1;
			iclut[i].c.g = 1;
			iclut[i].c.b = 1;
		}
	}

	graph_load_texture(&idata, &font->info);
}

typedef void
(*graph__font_char_emitter_f) (
	void *ctx,
	graph_point_t *pos,
	graph_texture_info_t *tex
	);

static void
graph__print(
	void *emitter_ctx,
	graph__font_char_emitter_f emitter,
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	)
{
	uint32_t
		offset_x    = 0,
		offset_y    = 0,
		tab_width   = font->cw * GRAPH_FONT_TAB_WIDTH,
		space_width = font->cw,
		line_height = font->ch;

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
		uint32_t cx = idx % font->cols * font->cw;
		uint32_t cy = idx / font->cols * font->ch;

		graph_texture_info_t char_tex = {
			.u    = font->info.u + cx,
			.v    = font->info.v + cy,
			.w    = font->cw,
			.h    = font->ch,
			.page = font->info.page,
			.clut = font->info.clut
		};

		emitter(
			emitter_ctx,
			&GRAPH_POINT(pos->x + offset_x, pos->y + offset_y),
			&char_tex
		);

		offset_x += font->cw;
	}
}

/*
 * DIRECTDRAW
 */

static void
graph__d_emit_glyph(
	void *ctx,
	graph_point_t *pos,
	graph_texture_info_t *tex
	)
{
	(void)ctx;
	graph_d_sprite(pos, tex, true, true);
}

void
graph_d_print_str(
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	)
{
	graph_d_set_page(font->info.page, false, false);
	graph__print(NULL, graph__d_emit_glyph, font, pos, str);
}

/*
 * DMADRAW
 */

static void
graph__m_emit_glyph(
	void *ctx,
	graph_point_t *pos,
	graph_texture_info_t *tex
	)
{
	graph__m_emit_ctx_t *c = ctx;
	graph_m_sprite(c->chain, pos, tex, true, true);
}

void
graph_m_print_str(
	graph_dma_chain_t *chain,
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	)
{
	graph__m_emit_ctx_t ctx = {
		.chain = chain,
	};

	graph_m_set_page(chain, font->info.page, false, false);
	graph__print(&ctx, graph__m_emit_glyph, font, pos, str);
}

/*
 * DMADRAW_ORD
 */

static void
graph__o_emit_glyph(
	void *ctx,
	graph_point_t *pos,
	graph_texture_info_t *tex
	)
{
	graph__o_emit_ctx_t *c = ctx;
	graph_o_sprite(c->chain, c->z, pos, tex, true, true);
}

void
graph_o_print_str(
	graph_dma_ord_chain_t *chain,
	uint32_t z,
	graph_font_t *font,
	graph_point_t *pos,
	const char *str
	)
{
	graph__o_emit_ctx_t ctx = {
		.chain = chain,
		.z = z
	};

	graph_o_set_page(chain, z, font->info.page, false, false);
	graph__print(&ctx, graph__o_emit_glyph, font, pos, str);
}
