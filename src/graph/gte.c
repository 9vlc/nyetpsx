/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/cop0.h>
#include <sys/gte.h>

#include <graph/gte.h>
#include <graph/gpu.h>

#include <trig.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

uint32_t
floor_log2(
	int32_t x
	)
{
	assert(x > 0);

	gte_set_data_reg(GTE_LZCS, x);
	gte_load_delay();

	return (31 - gte_get_data_reg(GTE_LZCR));
}

uint32_t
ceil_log2(
	int32_t x
	)
{
	assert(x > 0);

	gte_set_data_reg(GTE_LZCS, x - 1);
	gte_load_delay();

	return (32 - gte_get_data_reg(GTE_LZCR));
}

void
graph_init_gte(
	uint32_t width,
	uint32_t height
	)
{
	cop0_set_reg(COP0_STATUS, cop0_get_reg(COP0_STATUS) | COP0_STATUS_CU2);

	gte_set_ctrl_reg(GTE_OFX, (width  << 16) / 2);
	gte_set_ctrl_reg(GTE_OFY, (height << 16) / 2);

	int focal = ((width < height) ? width : height);

	gte_set_ctrl_reg(GTE_H, focal / 2);
	gte_set_ctrl_reg(GTE_ZSF3, GRAPH_DMA_CHAIN_OT_SIZE / 3);
	gte_set_ctrl_reg(GTE_ZSF4, GRAPH_DMA_CHAIN_OT_SIZE / 4);
}

#define MULTIPLY_MATRIX(FLAG, NAME) \
void \
graph_mul_##NAME(gte_mtx_t *out) \
{ \
	gte_cmd(GTE_CMD_MVMVA | GTE_SF | (FLAG) | GTE_V_V0 | \
		GTE_CV_NONE); \
	out->v[0][0] = (int16_t)(gte_get_data_reg(GTE_IR1)); \
	out->v[1][0] = (int16_t)(gte_get_data_reg(GTE_IR2)); \
	out->v[2][0] = (int16_t)(gte_get_data_reg(GTE_IR3)); \
	\
	gte_cmd(GTE_CMD_MVMVA | GTE_SF | (FLAG) | GTE_V_V1 | \
		GTE_CV_NONE); \
	out->v[0][1] = (int16_t)(gte_get_data_reg(GTE_IR1)); \
	out->v[1][1] = (int16_t)(gte_get_data_reg(GTE_IR2)); \
	out->v[2][1] = (int16_t)(gte_get_data_reg(GTE_IR3)); \
	\
	gte_cmd(GTE_CMD_MVMVA | GTE_SF | (FLAG) | GTE_V_V2 | \
		GTE_CV_NONE); \
	out->v[0][2] = (int16_t)(gte_get_data_reg(GTE_IR1)); \
	out->v[1][2] = (int16_t)(gte_get_data_reg(GTE_IR2)); \
	out->v[2][2] = (int16_t)(gte_get_data_reg(GTE_IR3)); \
}

MULTIPLY_MATRIX(GTE_MX_RT, rot_mtx)
MULTIPLY_MATRIX(GTE_MX_LLM, light_mtx)
MULTIPLY_MATRIX(GTE_MX_LCM, light_col_mtx)

#undef MULTIPLY_MATRIX

#define ROTATE_MATRIX(V11, V12, V13, V21, V22, V23, V31, V32, V33, NAME) \
void \
graph_rot_mtx_##NAME(int a) \
{ \
	int s = isin(a); \
	int c = icos(a); \
	\
	gte_set_col_vecs( \
		V11, V12, V13, \
		V21, V22, V23, \
		V31, V32, V33 \
	); \
	\
	gte_mtx_t result; \
	\
	graph_mul_rot_mtx(&result); \
	gte_load_rot_mtx(&result); \
}

ROTATE_MATRIX(
	GRAPH_GTE_UNIT, 0, 0,
			0, c, -s,
			0, s,  c,
	x
)

ROTATE_MATRIX(
	c, 0, s,
	0, GRAPH_GTE_UNIT, 0,
	-s, 0, c,
	y
)

ROTATE_MATRIX(
	c, -s, 0,
	s,  c, 0,
	0,  0, GRAPH_GTE_UNIT,
	z
)

#undef ROTATE_MATRIX

void
graph_tf_light_mtx(
	void
	)
{
	gte_mtx_t result;

	gte_store_rot_mtx(&result);
	gte_set_col_vecs(
		result.v[0][0], result.v[0][1], result.v[0][2],
		result.v[1][0], result.v[1][1], result.v[1][2],
		result.v[2][0], result.v[2][1], result.v[2][2]
	);

	graph_mul_light_mtx(&result);
	gte_load_light_mtx(&result);
}
