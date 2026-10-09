/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GEOM_H
#define _GRAPH__GEOM_H

#include <stdint.h>

/* I really hope the compiler doesn't break this */
union graph_rgb {
	struct { uint8_t r, g, b; } c;
	uint32_t raw;
};
typedef union graph_rgb graph_rgb_t;
#define GRAPH_RGB(R, G, B) (graph_rgb_t){.c.r=(R), .c.g=(G), .c.b=(B)}
#define GRAPH_XRGB(X) (graph_rgb_t){.raw=gp0_xrgb(X)}

struct graph_point {
	int32_t x, y;
};
typedef struct graph_point graph_point_t;
#define GRAPH_POINT(X, Y) (graph_point_t){.x=(X), .y=(Y)}

struct graph_line {
	struct graph_point p0, p1;
};
typedef struct graph_line graph_line_t;
#define GRAPH_LINE(X0, Y0, X1, Y1) (graph_line_t){ \
	.p0.x=(X0), .p0.y=(Y0), .p1.x=(X1), .p1.y=(Y1)}

struct graph_rect {
	int32_t x, y, w, h;
};
typedef struct graph_rect graph_rect_t;
#define GRAPH_RECT(X, Y, W, H) (graph_rect_t){.x=(X), .y=(Y), .w=(W), .h=(H)}

struct graph_tri {
	struct graph_point p0, p1, p2;
};
typedef struct graph_tri graph_tri_t;
#define GRAPH_TRI(X0, Y0, X1, Y1, X2, Y2) (graph_tri_t){ \
	.p0.x=(X0), .p0.y=(Y0), .p1.x=(X1), .p1.y=(Y1), \
	.p2.x=(X2), .p2.y=(Y2)}

struct graph_quad {
	struct graph_point p0, p1, p2, p3;
};
typedef struct graph_quad graph_quad_t;
#define GRAPH_QUAD(X0, Y0, X1, Y1, X2, Y2, X3, Y3) (graph_quad_t){ \
	.p0.x=(X0), .p0.y=(Y0), .p1.x=(X1), .p1.y=(Y1), \
	.p2.x=(X2), .p2.y=(Y2), .p3.x=(X3), .p3.y=(Y3)}

struct graph_uv {
	uint8_t u, v;
};
typedef struct graph_uv graph_uv_t;
#define GRAPH_UV(U, V) (graph_uv_t){.u=(U), .v=(V)}

#endif /* _GRAPH__GEOM_H */
