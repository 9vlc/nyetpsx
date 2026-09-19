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

/*
 * Structs
 */

union graph_rgb {
	struct { uint8_t b, g, r; };
	uint32_t raw;
};
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

/*
 * Inline functions
 */

DEF(void)
graph_rect(
	graph_rect_t rect,
	graph_rgb_t col
	)
{
	graph_wait_for_gpu();
	GPU_GP0 = col.raw | gp0_vram_fill();
	GPU_GP0 = gp0_xy(rect.x, rect.y);
	GPU_GP0 = gp0_xy(rect.w, rect.h);
}

DEF(void)
graph_shaded_tri(
	graph_tri_t tri,
	graph_rgb_t col1,
	graph_rgb_t col2,
	graph_rgb_t col3
	)
{
	graph_wait_for_gpu();
	GPU_GP0 = col1.raw | gp0_shaded_triangle(true, false, false);
	GPU_GP0 = gp0_xy(tri.p1.x, tri.p1.y);
	GPU_GP0 = col2.raw;
	GPU_GP0 = gp0_xy(tri.p2.x, tri.p2.y);
	GPU_GP0 = col3.raw;
	GPU_GP0 = gp0_xy(tri.p3.x, tri.p3.y);
}


#undef DEF

#endif /* _GRAPH__GEOM_H */
