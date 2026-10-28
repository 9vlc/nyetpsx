/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__DMADRAW_H
#define _GRAPH__DMADRAW_H

#include <graph/gpu.h>
#include <graph/geom.h>

#include <stdbool.h>
#include <stdint.h>

uint32_t *
graph_m_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_shad_tri(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	);

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
	);

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
	);

uint32_t *
graph_m_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_shad_quad(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	);

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
	);

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
	);

uint32_t *
graph_m_line(
	graph_dma_chain_t *chain,
	graph_line_t *line,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_shad_line(
	graph_dma_chain_t *chain,
	graph_line_t *line,
	graph_rgb_t col0,
	graph_rgb_t col1,
	bool blend
	);

uint32_t *
graph_m_line3(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_shad_line3(
	graph_dma_chain_t *chain,
	graph_tri_t *tri,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	);

uint32_t *
graph_m_line4(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_shad_line4(
	graph_dma_chain_t *chain,
	graph_quad_t *quad,
	graph_rgb_t col0,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	);

uint32_t *
graph_m_rect(
	graph_dma_chain_t *chain,
	graph_rect_t *rect,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_rect1x1(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_rect8x8(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_rect16x16(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_rgb_t col,
	bool blend
	);

uint32_t *
graph_m_sprite(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	);

uint32_t *
graph_m_sprite8x8(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	);

uint32_t *
graph_m_sprite16x16(
	graph_dma_chain_t *chain,
	graph_point_t *pos,
	graph_texture_info_t *tex,
	bool raw,
	bool blend
	);

uint32_t *
graph_m_end(
	graph_dma_chain_t *chain
	);

#endif /* _GRAPH__DMADRAW_H */
