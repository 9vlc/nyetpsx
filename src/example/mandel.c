/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 *
 * https://github.com/jesper-olsen/mandelbrot-c/blob/main/mandelbrot.c
 */

#include <graph/gpu.h>

#include <stdbool.h>
#include "example.h"

typedef struct {
	uint8_t i, r, g, b;
} color_point_t;

static const color_point_t clut_points[] = {
	{   0, 7,  13,  1 }, /* Dark green */
	{  64, 17, 21,  3 }, /* Light green */
	{ 128, 30, 28,  7 }, /* Yellow */
	{ 192, 31, 15,  3 }, /* Orange */
	{ 254, 15,  5,  1 }, /* Brown */
	{ 255, 17,  9,  2 }  /* Cube edges */
};
#define NUM_CLUT_POINTS (sizeof(clut_points) / sizeof(clut_points[0]))

/*
 * Wrapper for mult because libgcc functions kept getting generatred
 */
static inline int32_t __attribute__((always_inline))
mult(
	int32_t a,
	int32_t b
	)
{
	uint32_t lo;
	int32_t hi;

	__asm__(
		"mult %2, %3\n\t"
		"mflo %0\n\t"
		"mfhi %1\n\t"
		: "=r" (lo), "=r" (hi)
		: "r"  (a),  "r"  (b)
		: "hi", "lo"
	);

	return ((hi << (32 - MANDEL_SHIFT)) | (lo >> MANDEL_SHIFT));
}

/*
 * Check if the point is in the bulb
 */
static bool
is_in_bulb(
	int32_t cr,
	int32_t ci
	)
{
	int32_t ci2 = mult(ci, ci);

	int32_t xr = cr + MANDEL_UNIT;
	if (mult(xr, xr) + ci2 < (MANDEL_UNIT >> 4)) {
		return (true);
	}

	int32_t xq = cr - (MANDEL_UNIT >> 2);
	int32_t q = mult(xq, xq) + ci2;
	if (mult(q, q + xq) < (ci2 >> 2)) {
		return (true);
	}

	return (false);
}

/*
 * Calculate how long does it take for the function to escape
 */
static uint32_t
escape_time(
	int32_t cr,
	int32_t ci,
	uint32_t max_iter
	)
{
	if (is_in_bulb(cr, ci)) {
		return (max_iter);
	}

	int32_t	zr = 0,
		zi = 0;

	uint32_t iter;
	for (iter = 0; iter < max_iter; iter++) {
		int32_t zri = mult(zr, zi);
		int32_t zr2 = mult(zr, zr);
		int32_t zi2 = mult(zi, zi);

		if (zr2 + zi2 > MANDEL_ETIME) {
			break;
		}

		zi = (zri << 1) + ci;
		zr = zr2 - zi2 + cr;
	}

	return (iter);
}

void
mandel_render_line(
	uint8_t *fbuf,
	uint32_t width,
	uint32_t height,
	uint32_t y,
	int32_t ll_x,
	int32_t ll_y,
	int32_t ur_x,
	int32_t ur_y,
	uint32_t max_iter
	)
{
	int32_t step_x = (ur_x - ll_x) / width;
	int32_t step_y = (ur_y - ll_y) / height;
	uint32_t color_scale = (255 << 16) / max_iter;

	uint8_t *pixel = fbuf;

	int32_t imag = ur_y - step_y * y;
	int32_t real = ll_x;
	for (uint32_t x = 0; x < width; x++) {
		uint32_t iter = escape_time(real, imag, max_iter);

		*pixel++ = (iter * color_scale) >> 16;
		real += step_x;
	}
}

void
mandel_generate_clut(
	graph_rgb16_t *clut
	)
{
	for (int p = 0, i = 0; i < 256; i++) {
		if (i > clut_points[p + 1].i && (p + 2) < NUM_CLUT_POINTS) {
			p++;
		}

		int	span = clut_points[p + 1].i - clut_points[p].i,
			pos  = i - clut_points[p].i;

		uint8_t	r = clut_points[p].r,
			g = clut_points[p].g,
			b = clut_points[p].b;

		if (span > 0) {
			r += (clut_points[p + 1].r - r) * pos / span;
			g += (clut_points[p + 1].g - g) * pos / span;
			b += (clut_points[p + 1].b - b) * pos / span;
		}

		clut[i] = GRAPH_RGB16(r, g, b);
	}
}
