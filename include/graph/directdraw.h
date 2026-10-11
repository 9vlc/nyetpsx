/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__DIRECTDRAW_H
#define _GRAPH__DIRECTDRAW_H

#include <graph/gpu.h>
#include <graph/geom.h>

#include <stdbool.h>
#include <stdint.h>

void
graph_d_tri(
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_shad_tri(
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	);

void
graph_d_tex_tri(
	graph_tri_t *tri,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv0,
	graph_uv_t *uv1,
	graph_uv_t *uv2,
	bool blend
	);

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
	);

void
graph_d_quad(
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_shad_quad(
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	);

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
	);

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
	);

void
graph_d_line(
	graph_line_t *line,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_shad_line(
	graph_line_t *line,
	graph_rgb_t col0,
	graph_rgb_t col1,
	bool blend
	);

void
graph_d_line3(
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_shad_line3(
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	);

void
graph_d_line4(
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_shad_line4(
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	);

void
graph_d_rect(
	graph_rect_t *rect,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_rect1x1(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_rect8x8(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_rect16x16(
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

void
graph_d_sprite(
	graph_rect_t *rect,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv,
	bool raw,
	bool blend
	);

void
graph_d_sprite8x8(
	graph_point_t *pos,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv,
	bool raw,
	bool blend
	);

void
graph_d_sprite16x16(
	graph_point_t *pos,
	graph_rgb_t col,
	graph_texture_info_t *tex,
	graph_uv_t *uv,
	bool raw,
	bool blend
	);

#endif /* _GRAPH__DIRECTDRAW_H */
