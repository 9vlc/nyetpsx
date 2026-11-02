/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GTE_H
#define _GRAPH__GTE_H

#include <psx/gte.h>

#include <stdint.h>

#define GTE_SQRT2  (GTE_UNIT * 141421 / 100000)
#define GTE_RSQRT2 (GTE_UNIT *  70711 / 100000)
#define GTE_SQRT3  (GTE_UNIT * 173205 / 100000)
#define GTE_RSQRT3 (GTE_UNIT *  57735 / 100000)

uint32_t gte_floor_log2(int32_t x);
uint32_t gte_ceil_log2(int32_t x);

void
graph_init_gte(
	uint32_t width,
	uint32_t height
	);

void gte_mul_rot_mtx(gte_mtx_t *out);
void gte_mul_light_mtx(gte_mtx_t *out);
void gte_mul_light_col_mtx(gte_mtx_t *out);

void gte_rot_mtx_x(int a);
void gte_rot_mtx_y(int a);
void gte_rot_mtx_z(int a);

void gte_tf_light_mtx(void);

#endif /* _GRAPH__GTE_H */
