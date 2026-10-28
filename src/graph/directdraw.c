/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/geom.h>
#include <graph/font.h>

#include <stdbool.h>
#include <stdint.h>

void
graph_d_tri(
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_tri(false, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
}

void
graph_d_shad_tri(
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_shad_tri(true, false, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
}

void
graph_d_tex_tri(
	graph_tri_t *tri,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_tri(true, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = gp0_uv(uv0->u, uv0->v, tex->clut);
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = gp0_uv(uv1->u, uv1->v, tex->page);
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
	GPU_GP0 = gp0_uv(uv2->u, uv2->v, 0);
}

void
graph_d_shad_tex_tri(
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
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_shad_tri(true, true, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = gp0_uv(uv0->u, uv0->v, tex->clut);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = gp0_uv(uv1->u, uv1->v, tex->page);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
	GPU_GP0 = gp0_uv(uv2->u, uv2->v, 0);
}

void
graph_d_quad(
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_quad(false, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
}

void
graph_d_shad_quad(
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_shad_quad(true, false, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
}

void
graph_d_tex_quad(
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
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_quad(true, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = gp0_uv(uv0->u, uv0->v, tex->clut);
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = gp0_uv(uv1->u, uv1->v, tex->page);
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = gp0_uv(uv2->u, uv2->v, 0);
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
	GPU_GP0 = gp0_uv(uv3->u, uv3->v, 0);
}

void
graph_d_shad_tex_quad(
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
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_shad_quad(true, true, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = gp0_uv(uv0->u, uv0->v, tex->clut);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = gp0_uv(uv1->u, uv1->v, tex->page);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = gp0_uv(uv2->u, uv2->v, 0);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
	GPU_GP0 = gp0_uv(uv3->u, uv3->v, 0);
}

void
graph_d_line(
	graph_line_t *line,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_line(false, blend);
	GPU_GP0 = gp0_xy(line->p0.x, line->p0.y);
	GPU_GP0 = gp0_xy(line->p1.x, line->p1.y);
}

void
graph_d_shad_line(
	graph_line_t *line,
	graph_rgb_t col0,
	graph_rgb_t col1,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_line(true, blend);
	GPU_GP0 = gp0_xy(line->p0.x, line->p0.y);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(line->p1.x, line->p1.y);
}

void
graph_d_line3(
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_poly_line(false, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
	GPU_GP0 = 0x55555555;
}

void
graph_d_shad_line3(
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_poly_line(true, blend);
	GPU_GP0 = gp0_xy(tri->p0.x, tri->p0.y);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(tri->p1.x, tri->p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri->p2.x, tri->p2.y);
	GPU_GP0 = 0x55555555;
}

void
graph_d_line4(
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_poly_line(false, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
	GPU_GP0 = 0x55555555;
}

void
graph_d_shad_line4(
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col0.raw | gp0_poly_line(true, blend);
	GPU_GP0 = gp0_xy(quad->p0.x, quad->p0.y);
	GPU_GP0 = col1.raw;
	GPU_GP0 = gp0_xy(quad->p1.x, quad->p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad->p2.x, quad->p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad->p3.x, quad->p3.y);
	GPU_GP0 = 0x55555555;
}

void
graph_d_rect(
	graph_rect_t *rect,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_rect(false, false, blend);
	GPU_GP0 = gp0_xy(rect->x, rect->y);
	GPU_GP0 = gp0_xy(rect->w, rect->h);
}

void
graph_d_rect1x1(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_rect1x1(false, false, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
}

void
graph_d_rect8x8(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_rect8x8(false, false, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
}

void
graph_d_rect16x16(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = col.raw | gp0_rect16x16(false, false, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
}

void
graph_d_sprite(
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex->page;
	GPU_GP0 = gp0_rect(true, raw, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
	GPU_GP0 = gp0_uv(tex->u, tex->v, tex->clut);
	GPU_GP0 = gp0_xy(tex->w, tex->h);
}

void
graph_d_sprite8x8(
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex->page;
	GPU_GP0 = gp0_rect8x8(true, raw, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
	GPU_GP0 = gp0_uv(tex->u, tex->v, tex->clut);
}

void
graph_d_sprite16x16(
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	)
{
	gpu_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex->page;
	GPU_GP0 = gp0_rect16x16(true, raw, blend);
	GPU_GP0 = gp0_xy(pos->x, pos->y);
	GPU_GP0 = gp0_uv(tex->u, tex->v, tex->clut);
}

void
graph_d_print_str(
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

	gpu_await_cmd();
	GPU_GP0 = gp0_set_page(font->info.page, false, false);

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

		graph_d_sprite(
			&GRAPH_POINT(pos->x + offset_x, pos->y + offset_y),
			&char_tex,
			true, true
		);

		offset_x += font->cw;
	}
}
