/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GTE_H
#define _GRAPH__GTE_H

#include <psx/gte.h>

#include <stdint.h>

#define GRAPH_GTE_UNIT (1 << 12)

uint32_t floor_log2(int32_t x);
uint32_t ceil_log2(int32_t x);

void
graph_init_gte(
	uint32_t width,
	uint32_t height
	);

void graph_mul_rot_mtx(gte_mtx_t *out);
void graph_mul_light_mtx(gte_mtx_t *out);
void graph_mul_light_col_mtx(gte_mtx_t *out);

void graph_rot_mtx_x(int a);
void graph_rot_mtx_y(int a);
void graph_rot_mtx_z(int a);

void graph_tf_light_mtx(void);

#endif /* _GRAPH__GTE_H */
