/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__GTE_H
#define _SYS__GTE_H

#include <sys/mmio.h>

#include <stdint.h>

#define DEF(T) static inline T __attribute__((always_inline))
#define ALIGNED(A) __attribute__((aligned(A)))

/*
 * GTE data types
 */

/*
 * The GTE stores 16-bit vectors and matrices in 32-bit registers, packing two
 * values into each register. Since lwc2/swc2 can only perform aligned memory
 * accesses (see gte_load_data_reg() and gte_store_data_reg()), the structures below
 * must always be aligned to 32 bits.
 */
struct gte_vec16 {
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t _padding;
} ALIGNED(4);
typedef struct gte_vec16 gte_vec16_t;

struct gte_vec32 {
	int32_t x;
	int32_t y;
	int32_t z;
} ALIGNED(4);
typedef struct gte_vec32 gte_vec32_t;

struct gte_mtx {
	int16_t v[3][3];
	int16_t _padding;
} ALIGNED(4);
typedef struct gte_mtx gte_mtx_t;

/*
 * Command definitions
 */
#define GTE_CMD_BITMASK	(63u << 0)
#define GTE_CMD_RTPS	(1u << 0) /* Perspective transformation (1 vertex) */
#define GTE_CMD_NCLIP	(6u << 0) /* Normal clipping */
#define GTE_CMD_OP	(12u << 0) /* Outer product */
#define GTE_CMD_DPCS	(16u << 0) /* Depth cue (1 vertex) */
#define GTE_CMD_INTPL	(17u << 0) /* Depth cue with vector */
#define GTE_CMD_MVMVA	(18u << 0) /* Matrix-vector multiplication */
#define GTE_CMD_NCDS	(19u << 0) /* Normal color depth (1 vertex) */
#define GTE_CMD_CDP	(20u << 0) /* Color depth cue */
#define GTE_CMD_NCDT	(22u << 0) /* Normal color depth (3 vertices) */
#define GTE_CMD_NCCS	(27u << 0) /* Normal color color (1 vertex) */
#define GTE_CMD_CC	(28u << 0) /* Color color */
#define GTE_CMD_NCS	(30u << 0) /* Normal color (1 vertex) */
#define GTE_CMD_NCT	(32u << 0) /* Normal color (3 vertices) */
#define GTE_CMD_SQR	(40u << 0) /* Square of vector */
#define GTE_CMD_DCPL	(41u << 0) /* Depth cue with light */
#define GTE_CMD_DPCT	(42u << 0) /* Depth cue (3 vertices) */
#define GTE_CMD_AVSZ3	(45u << 0) /* Average Z value (3 vertices) */
#define GTE_CMD_AVSZ4	(46u << 0) /* Average Z value (4 vertices) */
#define GTE_CMD_RTPT	(48u << 0) /* Perspective transformation (3 vertices) */
#define GTE_CMD_GPF	(61u << 0) /* Linear interpolation */
#define GTE_CMD_GPL	(62u << 0) /* Linear interpolation with base */
#define GTE_CMD_NCCT	(63u << 0) /* Normal color color (3 vertices) */
#define GTE_LM		(1u << 10) /* Saturate IR to 0x0000-0x7FFF */
#define GTE_CV_BITMASK	(3u << 13)
#define GTE_CV_TR	(0u << 13) /* Use TR as translation vector for MVMVA */
#define GTE_CV_BK	(1u << 13) /* Use BK as translation vector for MVMVA */
#define GTE_CV_FC	(2u << 13) /* Use FC as translation vector for MVMVA */
#define GTE_CV_NONE	(3u << 13) /* Skip translation for MVMVA */
#define GTE_V_BITMASK	(3u << 15)
#define GTE_V_V0	(0u << 15) /* Use V0 as operand for MVMVA */
#define GTE_V_V1	(1u << 15) /* Use V1 as operand for MVMVA */
#define GTE_V_V2	(2u << 15) /* Use V2 as operand for MVMVA */
#define GTE_V_IR	(3u << 15) /* Use IR as operand for MVMVA */
#define GTE_MX_BITMASK	(3u << 17)
#define GTE_MX_RT	(0u << 17) /* Use rotation matrix as operand for MVMVA */
#define GTE_MX_LLM	(1u << 17) /* Use light matrix as operand for MVMVA */
#define GTE_MX_LCM	(2u << 17) /* Use light color matrix as operand for MVMVA */
#define GTE_SF		(1u << 19) /* Shift results by 12 bits */
typedef uint32_t gte_cmd_t;

/*
 * At least two dummy cycles are required after the last register write prior to
 * issuing a command (or before reading the result directly in the case of ORGB
 * or LZCR).
 */
DEF(void)
gte_load_delay(
	void
	)
{
	__asm__ volatile(
		"nop\n"
		"nop\n"
	);
}

DEF(void)
gte_cmd(
	const uint32_t cmd
	)
{
	gte_load_delay();
	__asm__ volatile("cop2 %0\n" :: "i"(cmd));
}

/*
 * Control register definitions
 */
#define GTE_RT11RT12	0 /* Rotation matrix */
#define GTE_RT13RT21	1 /* Rotation matrix */
#define GTE_RT22RT23	2 /* Rotation matrix */
#define GTE_RT31RT32	3 /* Rotation matrix */
#define GTE_RT33	4 /* Rotation matrix */
#define GTE_TRX		5 /* Translation vector */
#define GTE_TRY		6 /* Translation vector */
#define GTE_TRZ		7 /* Translation vector */
#define GTE_L11L12	8 /* Light matrix */
#define GTE_L13L21	9 /* Light matrix */
#define GTE_L22L23	10 /* Light matrix */
#define GTE_L31L32	11 /* Light matrix */
#define GTE_L33		12 /* Light matrix */
#define GTE_RBK		13 /* Background color */
#define GTE_GBK		14 /* Background color */
#define GTE_BBK		15 /* Background color */
#define GTE_LC11LC12	16 /* Light color matrix */
#define GTE_LC13LC21	17 /* Light color matrix */
#define GTE_LC22LC23	18 /* Light color matrix */
#define GTE_LC31LC32	19 /* Light color matrix */
#define GTE_LC33	20 /* Light color matrix */
#define GTE_RFC		21 /* Far color */
#define GTE_GFC		22 /* Far color */
#define GTE_BFC		23 /* Far color */
#define GTE_OFX		24 /* Screen coordinate offset */
#define GTE_OFY		25 /* Screen coordinate offset */
#define GTE_H		26 /* Projection plane distance */
#define GTE_DQA		27 /* Depth cue scale factor */
#define GTE_DQB		28 /* Depth cue base */
#define GTE_ZSF3	29 /* Average Z scale factor */
#define GTE_ZSF4	30 /* Average Z scale factor */
#define GTE_FLAG	31 /* Error/overflow flags */
typedef uint32_t gte_ctrl_reg_t;

/*
 * Status flag definitions
 */
#define GTE_FLAG_IR0_SATURATED		(1u << 12)
#define GTE_FLAG_SY2_SATURATED		(1u << 13)
#define GTE_FLAG_SX2_SATURATED		(1u << 14)
#define GTE_FLAG_MAC0_UNDERFLOW		(1u << 15)
#define GTE_FLAG_MAC0_OVERFLOW		(1u << 16)
#define GTE_FLAG_DIVIDE_OVERFLOW	(1u << 17)
#define GTE_FLAG_Z_SATURATED		(1u << 18)
#define GTE_FLAG_B_SATURATED		(1u << 19)
#define GTE_FLAG_G_SATURATED		(1u << 20)
#define GTE_FLAG_R_SATURATED		(1u << 21)
#define GTE_FLAG_IR3_SATURATED		(1u << 22)
#define GTE_FLAG_IR2_SATURATED		(1u << 23)
#define GTE_FLAG_IR1_SATURATED		(1u << 24)
#define GTE_FLAG_MAC3_UNDERFLOW		(1u << 25)
#define GTE_FLAG_MAC2_UNDERFLOW		(1u << 26)
#define GTE_FLAG_MAC1_UNDERFLOW		(1u << 27)
#define GTE_FLAG_MAC3_OVERFLOW		(1u << 28)
#define GTE_FLAG_MAC2_OVERFLOW		(1u << 29)
#define GTE_FLAG_MAC1_OVERFLOW		(1u << 30)
#define GTE_FLAG_ERROR			(1u << 31)
typedef uint32_t gte_flag_t;

/*
 * Note that reg must be a constant value known at compile time, as the
 * cfc2/ctc2 instructions only support addressing coprocessor registers directly
 * through immediates.
 */
DEF(void)
gte_set_ctrl_reg(
	gte_ctrl_reg_t reg,
	uint32_t value
	)
{
	__asm__ volatile("ctc2 %0, $%1\n" :: "r"(value), "i"(reg));
}

DEF(uint32_t)
gte_get_ctrl_reg(
	gte_ctrl_reg_t reg
	)
{
	uint32_t value;

	__asm__ volatile("cfc2 %0, $%1\n" : "=r"(value) : "i"(reg));
	return (value);
}

#define MATRIX_FUNCTIONS(REG0, REG1, REG2, REG3, REG4, NAME) \
	DEF(void) \
	gte_set_##NAME( \
		int16_t v11, int16_t v12, int16_t v13, \
		int16_t v21, int16_t v22, int16_t v23, \
		int16_t v31, int16_t v32, int16_t v33 \
		) \
	{ \
		gte_set_ctrl_reg(REG0, ((uint16_t)v11) | ((uint16_t)v12 << 16)); \
		gte_set_ctrl_reg(REG1, ((uint16_t)v13) | ((uint16_t)v21 << 16)); \
		gte_set_ctrl_reg(REG2, ((uint16_t)v22) | ((uint16_t)v23 << 16)); \
		gte_set_ctrl_reg(REG3, ((uint16_t)v31) | ((uint16_t)v32 << 16)); \
		gte_set_ctrl_reg(REG4, v33); \
	} \
	\
	DEF(void) \
	gte_load_##NAME(const gte_mtx_t *in) \
	{ \
		gte_set_ctrl_reg(REG0, ((uint16_t)in->v[0][0]) | ((uint16_t)in->v[0][1] << 16)); \
		gte_set_ctrl_reg(REG1, ((uint16_t)in->v[0][2]) | ((uint16_t)in->v[1][0] << 16)); \
		gte_set_ctrl_reg(REG2, ((uint16_t)in->v[1][1]) | ((uint16_t)in->v[1][2] << 16)); \
		gte_set_ctrl_reg(REG3, ((uint16_t)in->v[2][0]) | ((uint16_t)in->v[2][1] << 16)); \
		gte_set_ctrl_reg(REG4, ((uint16_t)in->v[2][2])); \
	} \
	\
	DEF(void) \
	gte_store_##NAME(gte_mtx_t *out) \
	{ \
		uint32_t v0 = gte_get_ctrl_reg(REG0); \
		uint32_t v1 = gte_get_ctrl_reg(REG1); \
		uint32_t v2 = gte_get_ctrl_reg(REG2); \
		uint32_t v3 = gte_get_ctrl_reg(REG3); \
		uint32_t v4 = gte_get_ctrl_reg(REG4); \
		\
		out->v[0][0] = (int16_t)(v0 >>  0); \
		out->v[0][1] = (int16_t)(v0 >> 16); \
		out->v[0][2] = (int16_t)(v1 >>  0); \
		out->v[1][0] = (int16_t)(v1 >> 16); \
		out->v[1][1] = (int16_t)(v2 >>  0); \
		out->v[1][2] = (int16_t)(v2 >> 16); \
		out->v[2][0] = (int16_t)(v3 >>  0); \
		out->v[2][1] = (int16_t)(v3 >> 16); \
		out->v[2][2] = (int16_t)(v4 >>  0); \
	}

MATRIX_FUNCTIONS(GTE_RT11RT12, GTE_RT13RT21, GTE_RT22RT23, GTE_RT31RT32, GTE_RT33, rot_mtx)
MATRIX_FUNCTIONS(GTE_L11L12, GTE_L13L21, GTE_L22L23, GTE_L31L32, GTE_L33, light_mtx)
MATRIX_FUNCTIONS(GTE_LC11LC12, GTE_LC13LC21, GTE_LC22LC23, GTE_LC31LC32, GTE_LC33, light_col_mtx)

#undef MATRIX_FUNCTIONS

/*
 * Data register definitions
 */
#define GTE_VXY0	0 /* Input vector 0 */
#define GTE_VZ0		1 /* Input vector 0 */
#define GTE_VXY1	2 /* Input vector 1 */
#define GTE_VZ1		3 /* Input vector 1 */
#define GTE_VXY2	4 /* Input vector 2 */
#define GTE_VZ2		5 /* Input vector 2 */
#define GTE_RGBC	6 /* Input color and GPU command */
#define GTE_OTZ		7 /* Average Z value out */
#define GTE_IR0		8 /* Scalar accumulator */
#define GTE_IR1		9 /* Vector accumulator */
#define GTE_IR2		10 /* Vector accumulator */
#define GTE_IR3		11 /* Vector accumulator */
#define GTE_SXY0	12 /* X/Y coordinate out FIFO */
#define GTE_SXY1	13 /* X/Y coordinate out FIFO */
#define GTE_SXY2	14 /* X/Y coordinate out FIFO */
#define GTE_SXYP	15 /* X/Y coordinate out FIFO */
#define GTE_SZ0		16 /* Z coordinate out FIFO */
#define GTE_SZ1		17 /* Z coordinate out FIFO */
#define GTE_SZ2		18 /* Z coordinate out FIFO */
#define GTE_SZ3		19 /* Z coordinate out FIFO */
#define GTE_RGB0	20 /* Color and GPU command out FIFO */
#define GTE_RGB1	21 /* Color and GPU command out FIFO */
#define GTE_RGB2	22 /* Color and GPU command out FIFO */
#define GTE_MAC0	24 /* Extended scalar accumulator */
#define GTE_MAC1	25 /* Extended vector accumulator */
#define GTE_MAC2	26 /* Extended vector accumulator */
#define GTE_MAC3	27 /* Extended vector accumulator */
#define GTE_IRGB	28 /* RGB conversion input */
#define GTE_ORGB	29 /* RGB conversion out */
#define GTE_LZCS	30 /* Leading zero count input */
#define GTE_LZCR	31 /* Leading zero count out */
typedef uint32_t gte_data_reg_t;

DEF(void)
gte_set_data_reg(
	gte_data_reg_t reg,
	uint32_t value
	)
{
	__asm__ volatile("mtc2 %0, $%1\n" :: "r"(value), "i"(reg));
}

DEF(uint32_t)
gte_get_data_reg(
	gte_data_reg_t reg
	)
{
	uint32_t value;

	__asm__ volatile("mfc2 %0, $%1\n" : "=r"(value) : "i"(reg));
	return (value);
}

/*
 * Unlike COP0 registers and GTE control registers, whose contents can only be
 * moved to/from a CPU register, data registers can additionally be loaded from
 * and stored to memory directly using the lwc2 and swc2 instructions.
 */
DEF(void)
gte_load_data_reg(
	gte_data_reg_t reg,
	int16_t offset,
	const void *ptr
	)
{
	__asm__ volatile("lwc2 $%0, %1(%2)\n" :: "i"(reg), "i"(offset), "r"(ptr));
}

DEF(void)
gte_store_data_reg(
	gte_data_reg_t reg,
	int16_t offset,
	void *ptr
	)
{
	__asm__ volatile("swc2 $%0, %1(%2)\n" :: "i"(reg), "i"(offset), "r"(ptr) : "memory");
}

#define VECTOR_FUNCTIONS(REG0, REG1, NAME) \
DEF(void) \
gte_set_##NAME( \
	int16_t x, \
	int16_t y, \
	int16_t z \
	) \
{ \
	gte_set_data_reg(REG0, ((uint16_t)x) | ((uint16_t)y << 16)); \
	gte_set_data_reg(REG1, z); \
} \
\
DEF(void) \
gte_load_##NAME(const gte_vec16_t *input) \
{ \
	gte_load_data_reg(REG0, 0, input); \
	gte_load_data_reg(REG1, 4, input); \
} \
\
DEF(void) \
gte_store_##NAME(gte_vec16_t *out) \
{ \
	gte_store_data_reg(REG0, 0, out); \
	gte_store_data_reg(REG1, 4, out); \
}

VECTOR_FUNCTIONS(GTE_VXY0, GTE_VZ0, V0)
VECTOR_FUNCTIONS(GTE_VXY1, GTE_VZ1, V1)
VECTOR_FUNCTIONS(GTE_VXY2, GTE_VZ2, V2)

#undef VECTOR_FUNCTIONS

DEF(void)
gte_set_row_vecs(
	int16_t v11, int16_t v12, int16_t v13,
	int16_t v21, int16_t v22, int16_t v23,
	int16_t v31, int16_t v32, int16_t v33
	)
{
	gte_set_data_reg(GTE_VXY0, ((uint16_t)v11) | ((uint16_t)v12 << 16));
	gte_set_data_reg(GTE_VZ0,  v13);
	gte_set_data_reg(GTE_VXY1, ((uint16_t)v21) | ((uint16_t)v22 << 16));
	gte_set_data_reg(GTE_VZ1,  v23);
	gte_set_data_reg(GTE_VXY2, ((uint16_t)v31) | ((uint16_t)v32 << 16));
	gte_set_data_reg(GTE_VZ2,  v33);
}

DEF(void)
gte_set_col_vecs(
	int16_t v11, int16_t v12, int16_t v13,
	int16_t v21, int16_t v22, int16_t v23,
	int16_t v31, int16_t v32, int16_t v33
	)
{
	gte_set_data_reg(GTE_VXY0, ((uint16_t)v11) | ((uint16_t)v21 << 16));
	gte_set_data_reg(GTE_VZ0,  v31);
	gte_set_data_reg(GTE_VXY1, ((uint16_t)v12) | ((uint16_t)v22 << 16));
	gte_set_data_reg(GTE_VZ1,  v32);
	gte_set_data_reg(GTE_VXY2, ((uint16_t)v13) | ((uint16_t)v23 << 16));
	gte_set_data_reg(GTE_VZ2,  v33);
}

#undef DEF
#undef ALIGNED

#endif /* _SYS__GTE_H */
