/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GEOM_H
#define _GRAPH__GEOM_H

#include <sys/gpu.h>

#include <graph/gpuctl.h>

#include <stdbool.h>
#include <stdint.h>

#define DEF(type) static inline type __attribute__((always_inline))
#define PACKED __attribute__((packed))

/*
 * Structs
 */

union graph_rgb {
	struct { uint8_t r, g, b; } PACKED;
	uint32_t raw;
} PACKED;
typedef union graph_rgb graph_rgb_t;
#define GRAPH_RGB(R, G, B) (graph_rgb_t){.r=(R), .g=(G), .b=(B)}
#define GRAPH_XRGB(X) (graph_rgb_t){.raw=gp0_xrgb(X)}

struct graph_point {
	int32_t x, y;
};
typedef struct graph_point graph_point_t;
#define GRAPH_POINT(X, Y) (graph_point_t){.x=(X), .y=(Y)}

struct graph_line {
	struct graph_point p1, p2;
};
typedef struct graph_line graph_line_t;
#define GRAPH_LINE(X1, Y1, X2, Y2) (graph_line_t){ \
	.p1.x=(X1), .p1.y=(Y1), .p2.x=(X2), .p2.y=(Y2)}

struct graph_rect {
	int32_t x, y, w, h;
};
typedef struct graph_rect graph_rect_t;
#define GRAPH_RECT(X, Y, W, H) (graph_rect_t){.x=(X), .y=(Y), .w=(W), .h=(H)}

struct graph_tri {
	struct graph_point p1, p2, p3;
};
typedef struct graph_tri graph_tri_t;
#define GRAPH_TRI(X1, Y1, X2, Y2, X3, Y3) (graph_tri_t){ \
	.p1.x=(X1), .p1.y=(Y1), .p2.x=(X2), .p2.y=(Y2), \
	.p3.x=(X3), .p3.y=(Y3)}

struct graph_quad {
	struct graph_point p1, p2, p3, p4;
};
typedef struct graph_quad graph_quad_t;
#define GRAPH_QUAD(X1, Y1, X2, Y2, X3, Y3, X4, Y4) (graph_quad_t){ \
	.p1.x=(X1), .p1.y=(Y1), .p2.x=(X2), .p2.y=(Y2), \
	.p3.x=(X3), .p3.y=(Y3), .p4.x=(X4), .p4.y=(Y4)}

struct graph_texture {
	uint16_t page, clut;
};
typedef struct graph_texture graph_texture_t;
#define GRAPH_TEXTURE(PAGE, CLUT) (graph_texture_t){.page=(PAGE), .clut=(CLUT)}

struct graph_uv {
	uint8_t u, v;
};
typedef struct graph_uv graph_uv_t;
#define GRAPH_UV(U, V) (graph_uv_t){.u=(U), .v=(V)}

/*
 * Direct draw
 */

DEF(void)
graph_d_tri(
	graph_tri_t tri,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_tri(false, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
}

DEF(void)
graph_d_shad_tri(
	graph_tri_t tri,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_shad_tri(true, false, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
}

DEF(void)
graph_d_tex_tri(
	graph_tri_t tri,
	graph_rgb_t col,
	graph_texture_t tex,
	graph_uv_t uv1,
	graph_uv_t uv2,
	graph_uv_t uv3,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_tri(true, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = gp0_uv(uv1.u, uv1.v, tex.clut);
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = gp0_uv(uv2.u, uv2.v, tex.page);
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
	GPU_GP0 = gp0_uv(uv3.u, uv3.v, 0);
}

DEF(void)
graph_d_shad_tex_tri(
	graph_tri_t tri,
	graph_texture_t tex,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	graph_uv_t uv1,
	graph_uv_t uv2,
	graph_uv_t uv3,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_shad_tri(true, true, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = gp0_uv(uv1.u, uv1.v, tex.clut);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = gp0_uv(uv2.u, uv2.v, tex.page);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
	GPU_GP0 = gp0_uv(uv3.u, uv3.v, 0);
}

DEF(void)
graph_d_quad(
	graph_quad_t quad,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_quad(false, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
}

DEF(void)
graph_d_shad_quad(
	graph_quad_t quad,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	graph_rgb_t col4,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_shad_quad(true, false, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = col4.raw;
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
}

DEF(void)
graph_d_tex_quad(
	graph_quad_t quad,
	graph_rgb_t col,
	graph_texture_t tex,
	graph_uv_t uv1,
	graph_uv_t uv2,
	graph_uv_t uv3,
	graph_uv_t uv4,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_quad(true, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = gp0_uv(uv1.u, uv1.v, tex.clut);
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = gp0_uv(uv2.u, uv2.v, tex.page);
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = gp0_uv(uv3.u, uv3.v, 0);
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
	GPU_GP0 = gp0_uv(uv4.u, uv4.v, 0);
}

DEF(void)
graph_d_shad_tex_quad(
	graph_quad_t quad,
	graph_texture_t tex,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	graph_rgb_t col4,
	graph_uv_t uv1,
	graph_uv_t uv2,
	graph_uv_t uv3,
	graph_uv_t uv4,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_shad_quad(true, true, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = gp0_uv(uv1.u, uv1.v, tex.clut);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = gp0_uv(uv2.u, uv2.v, tex.page);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = gp0_uv(uv3.u, uv3.v, 0);
	GPU_GP0 = col4.raw;
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
	GPU_GP0 = gp0_uv(uv4.u, uv4.v, 0);
}

DEF(void)
graph_d_line(
	graph_line_t line,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_line(false, blend);
	GPU_GP0 = gp0_xy(line.p1.x, line.p1.y);
	GPU_GP0 = gp0_xy(line.p2.x, line.p2.y);
}

DEF(void)
graph_d_shad_line(
	graph_line_t line,
	graph_rgb_t col1,
	graph_rgb_t col2,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_line(true, blend);
	GPU_GP0 = gp0_xy(line.p1.x, line.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(line.p2.x, line.p2.y);
}

DEF(void)
graph_d_line3(
	graph_tri_t tri,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_poly_line(false, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
	GPU_GP0 = 0x55555555;
}

DEF(void)
graph_d_shad_line3(
	graph_tri_t tri,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_poly_line(true, blend);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
	GPU_GP0 = 0x55555555;
}

DEF(void)
graph_d_line4(
	graph_quad_t quad,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_poly_line(false, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
	GPU_GP0 = 0x55555555;
}

DEF(void)
graph_d_shad_line4(
	graph_quad_t quad,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3,
	graph_rgb_t col4,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col1.raw | gp0_poly_line(true, blend);
	GPU_GP0 = gp0_xy(quad.p1.x, quad.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(quad.p2.x, quad.p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(quad.p3.x, quad.p3.y);
	GPU_GP0 = col4.raw;
	GPU_GP0 = gp0_xy(quad.p4.x, quad.p4.y);
	GPU_GP0 = 0x55555555;
}

DEF(void)
graph_d_rect(
	graph_rect_t rect,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_rect(false, false, blend);
	GPU_GP0 = gp0_xy(rect.x, rect.y);
	GPU_GP0 = gp0_xy(rect.w, rect.h);
}

DEF(void)
graph_d_rect1x1(
	graph_point_t pos,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_rect1x1(false, false, blend);
	GPU_GP0 = gp0_xy(pos.x, pos.y);
}

DEF(void)
graph_d_rect8x8(
	graph_point_t pos,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_rect8x8(false, false, blend);
	GPU_GP0 = gp0_xy(pos.x, pos.y);
}

DEF(void)
graph_d_rect16x16(
	graph_point_t pos,
	graph_rgb_t col,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = col.raw | gp0_rect16x16(false, false, blend);
	GPU_GP0 = gp0_xy(pos.x, pos.y);
}

DEF(void)
graph_d_sprite(
	graph_rect_t rect,
	graph_rgb_t col,
	graph_texture_t tex,
	graph_uv_t uv,
	bool raw,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex.page;
	GPU_GP0 = col.raw | gp0_rect(true, raw, blend);
	GPU_GP0 = gp0_xy(rect.x, rect.y);
	GPU_GP0 = gp0_uv(uv.u, uv.v, tex.clut);
	GPU_GP0 = gp0_xy(rect.w, rect.h);
}

DEF(void)
graph_d_sprite8x8(
	graph_point_t pos,
	graph_rgb_t col,
	graph_texture_t tex,
	graph_uv_t uv,
	bool raw,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex.page;
	GPU_GP0 = col.raw | gp0_rect8x8(true, raw, blend);
	GPU_GP0 = gp0_xy(pos.x, pos.y);
	GPU_GP0 = gp0_uv(uv.u, uv.v, tex.clut);
}

DEF(void)
graph_d_sprite16x16(
	graph_point_t pos,
	graph_rgb_t col,
	graph_texture_t tex,
	graph_uv_t uv,
	bool raw,
	bool blend
	)
{
	graph_await_cmd();

	GPU_GP0 = GP0_CMD_TPAGE | tex.page;
	GPU_GP0 = col.raw | gp0_rect16x16(true, raw, blend);
	GPU_GP0 = gp0_xy(pos.x, pos.y);
	GPU_GP0 = gp0_uv(uv.u, uv.v, tex.clut);
}

#undef DEF
#undef PACKED

#endif /* _GRAPH__GEOM_H */
