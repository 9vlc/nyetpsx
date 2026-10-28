/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/geom.h>
#include <graph/font.h>
#include <graph/dmadraw.h>

#include <stdbool.h>
#include <stdint.h>

uint32_t *
graph_m_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 4);

	ptr[0] = col.raw | gp0_tri(false, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[3] = gp0_xy(tri->p2.x, tri->p2.y);

	return (ptr);
}

uint32_t *
graph_m_shad_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 6);

	ptr[0] = col0.raw | gp0_shad_tri(true, false, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = col1.raw;
	ptr[3] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[4] = col2.raw;
	ptr[5] = gp0_xy(tri->p2.x, tri->p2.y);

	return (ptr);
}

uint32_t *
graph_m_tex_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 7);

	ptr[0] = col.raw | gp0_tri(true, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = gp0_uv(uv0->u, uv0->v, tex->clut);
	ptr[3] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[4] = gp0_uv(uv1->u, uv1->v, tex->page);
	ptr[5] = gp0_xy(tri->p2.x, tri->p2.y);
	ptr[6] = gp0_uv(uv2->u, uv2->v, 0);

	return (ptr);
}

uint32_t *
graph_m_shad_tex_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_texture_info_t *tex,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 9);

	ptr[0] = col0.raw | gp0_shad_tri(true, true, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = gp0_uv(uv0->u, uv0->v, tex->clut);
	ptr[3] = col1.raw;
	ptr[4] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[5] = gp0_uv(uv1->u, uv1->v, tex->page);
	ptr[6] = col2.raw;
	ptr[7] = gp0_xy(tri->p2.x, tri->p2.y);
	ptr[8] = gp0_uv(uv2->u, uv2->v, 0);

	return (ptr);
}

uint32_t *
graph_m_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 5);

	ptr[0] = col.raw | gp0_quad(false, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[3] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[4] = gp0_xy(quad->p3.x, quad->p3.y);

	return (ptr);
}

uint32_t *
graph_m_shad_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 8);

	ptr[0] = col0.raw | gp0_shad_quad(true, false, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = col1.raw;
	ptr[3] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[4] = col2.raw;
	ptr[5] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[6] = col3.raw;
	ptr[7] = gp0_xy(quad->p3.x, quad->p3.y);

	return (ptr);
}

uint32_t *
graph_m_tex_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	graph_uv_t *uv3,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 9);

	ptr[0] = col.raw | gp0_quad(true, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = gp0_uv(uv0->u, uv0->v, tex->clut);
	ptr[3] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[4] = gp0_uv(uv1->u, uv1->v, tex->page);
	ptr[5] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[6] = gp0_uv(uv2->u, uv2->v, 0);
	ptr[7] = gp0_xy(quad->p3.x, quad->p3.y);
	ptr[8] = gp0_uv(uv3->u, uv3->v, 0);

	return (ptr);
}

uint32_t *
graph_m_shad_tex_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_texture_info_t *tex,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	graph_uv_t *uv3,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 12);

	ptr[0] = col0.raw | gp0_shad_quad(true, true, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = gp0_uv(uv0->u, uv0->v, tex->clut);
	ptr[3] = col1.raw;
	ptr[4] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[5] = gp0_uv(uv1->u, uv1->v, tex->page);
	ptr[6] = col2.raw;
	ptr[7] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[8] = gp0_uv(uv2->u, uv2->v, 0);
	ptr[9] = col3.raw;
	ptr[10] = gp0_xy(quad->p3.x, quad->p3.y);
	ptr[11] = gp0_uv(uv3->u, uv3->v, 0);

	return (ptr);
}

uint32_t *
graph_m_line(
	graph_dma_chain_t *chain,
	graph_line_t *line,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 3);

	ptr[0] = col.raw | gp0_line(false, blend);
	ptr[1] = gp0_xy(line->p0.x, line->p0.y);
	ptr[2] = gp0_xy(line->p1.x, line->p1.y);

	return (ptr);
}

uint32_t *
graph_m_shad_line(
	graph_dma_chain_t *chain,
	graph_line_t *line,
	graph_rgb_t col0,
	graph_rgb_t col1,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 4);

	ptr[0] = col0.raw | gp0_line(true, blend);
	ptr[1] = gp0_xy(line->p0.x, line->p0.y);
	ptr[2] = col1.raw;
	ptr[3] = gp0_xy(line->p1.x, line->p1.y);

	return (ptr);
}

uint32_t *
graph_m_line3(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 5);

	ptr[0] = col.raw | gp0_poly_line(false, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[3] = gp0_xy(tri->p2.x, tri->p2.y);
	ptr[4] = 0x55555555;

	return (ptr);
}

uint32_t *
graph_m_shad_line3(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 7);

	ptr[0] = col0.raw | gp0_poly_line(true, blend);
	ptr[1] = gp0_xy(tri->p0.x, tri->p0.y);
	ptr[2] = col1.raw;
	ptr[3] = gp0_xy(tri->p1.x, tri->p1.y);
	ptr[4] = col2.raw;
	ptr[5] = gp0_xy(tri->p2.x, tri->p2.y);
	ptr[6] = 0x55555555;

	return (ptr);
}

uint32_t *
graph_m_line4(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 6);

	ptr[0] = col.raw | gp0_poly_line(false, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[3] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[4] = gp0_xy(quad->p3.x, quad->p3.y);
	ptr[5] = 0x55555555;

	return (ptr);
}

uint32_t *
graph_m_shad_line4(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 9);

	ptr[0] = col0.raw | gp0_poly_line(true, blend);
	ptr[1] = gp0_xy(quad->p0.x, quad->p0.y);
	ptr[2] = col1.raw;
	ptr[3] = gp0_xy(quad->p1.x, quad->p1.y);
	ptr[4] = col2.raw;
	ptr[5] = gp0_xy(quad->p2.x, quad->p2.y);
	ptr[6] = col3.raw;
	ptr[7] = gp0_xy(quad->p3.x, quad->p3.y);
	ptr[8] = 0x55555555;

	return (ptr);
}

uint32_t *
graph_m_rect(
	graph_dma_chain_t *chain,
	graph_rect_t *rect,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 3);

	ptr[0] = col.raw | gp0_rect(false, false, blend);
	ptr[1] = gp0_xy(rect->x, rect->y);
	ptr[2] = gp0_xy(rect->w, rect->h);

	return (ptr);
}

uint32_t *
graph_m_rect1x1(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 2);

	ptr[0] = col.raw | gp0_rect1x1(false, false, blend);
	ptr[1] = gp0_xy(pos->x, pos->y);

	return (ptr);
}

uint32_t *
graph_m_rect8x8(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 2);

	ptr[0] = col.raw | gp0_rect8x8(false, false, blend);
	ptr[1] = gp0_xy(pos->x, pos->y);

	return (ptr);
}

uint32_t *
graph_m_rect16x16(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 2);

	ptr[0] = col.raw | gp0_rect16x16(false, false, blend);
	ptr[1] = gp0_xy(pos->x, pos->y);

	return (ptr);
}

uint32_t *
graph_m_sprite(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 5);

	ptr[0] = GP0_CMD_TPAGE | tex->page;
	ptr[1] = gp0_rect(true, raw, blend);
	ptr[2] = gp0_xy(pos->x, pos->y);
	ptr[3] = gp0_uv(tex->u, tex->v, tex->clut);
	ptr[4] = gp0_xy(tex->w, tex->h);

	return (ptr);
}

uint32_t *
graph_m_sprite8x8(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 4);

	ptr[0] = GP0_CMD_TPAGE | tex->page;
	ptr[1] = gp0_rect8x8(true, raw, blend);
	ptr[2] = gp0_xy(pos->x, pos->y);
	ptr[3] = gp0_uv(tex->u, tex->v, tex->clut);

	return (ptr);
}

uint32_t *
graph_m_sprite16x16(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	uint32_t *ptr = graph_alloc_chain_packet(chain, 4);

	ptr[0] = GP0_CMD_TPAGE | tex->page;
	ptr[1] = gp0_rect16x16(true, raw, blend);
	ptr[2] = gp0_xy(pos->x, pos->y);
	ptr[3] = gp0_uv(tex->u, tex->v, tex->clut);

	return (ptr);
}

void
graph_m_print_str(
	graph_dma_chain_t *chain,
	graph_font_t *font,
	graph_point_t *pos,
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

		graph_m_sprite(
			chain,
			&GRAPH_POINT(pos->x + offset_x, pos->y + offset_y),
			&char_tex,
			true, true
		);

		offset_x += font->cw;
	}
}

uint32_t *
graph_m_end(
	graph_dma_chain_t *chain
	)
{
	*chain->next = gp0_end_tag(0);

	return (chain->next);
}
