/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__COP0_H
#define _SYS__COP0_H

#include <stdint.h>

#define DEF(T) static inline T __attribute__((always_inline))

#define COP0_BPC		3u  /* Breakpoint program counter (CW33300) */
#define COP0_BDA		5u  /* Breakpoint data address (CW33300) */
#define COP0_DCIC		7u  /* Debug and cache invalidation control (CW33300) */
#define COP0_BADVADDR		8u  /* Bad virtual address */
#define COP0_BDAM		9u  /* Breakpoint data address mask (CW33300) */
#define COP0_BPCM		11u /* Breakpoint program counter mask (CW33300) */
#define COP0_STATUS		12u /* Status register */
#define COP0_CAUSE		13u /* Exception cause */
#define COP0_EPC		14u /* Exception program counter */
#define COP0_PRID		15u /* Processor identifier */
typedef uint32_t cop0_reg_t;

#define COP0_DCIC_DB		(1u <<  0) /* Debug event pending */
#define COP0_DCIC_PC		(1u <<  1) /* Program counter breakpoint pending */
#define COP0_DCIC_DA		(1u <<  2) /* Data address breakpoint pending */
#define COP0_DCIC_R		(1u <<  3) /* Data address read breakpoint pending */
#define COP0_DCIC_W		(1u <<  4) /* Data address write breakpoint pending */
#define COP0_DCIC_T		(1u <<  5) /* Trace event pending */
#define COP0_DCIC_DE		(1u << 23) /* Debug enable */
#define COP0_DCIC_PCE		(1u << 24) /* Program counter breakpoint enable */
#define COP0_DCIC_DAE		(1u << 25) /* Data address breakpoint enable */
#define COP0_DCIC_DR		(1u << 26) /* Data address read breakpoint enable */
#define COP0_DCIC_DW		(1u << 27) /* Data address write breakpoint enable */
#define COP0_DCIC_TE		(1u << 28) /* Trace enable */
#define COP0_DCIC_KD		(1u << 29) /* Kernel debug enable */
#define COP0_DCIC_UD		(1u << 30) /* User debug enable */
#define COP0_DCIC_TR		(1u << 31) /* Debug event trap enable */
typedef uint32_t cop0_dcic_flag_t;

#define COP0_STATUS_IEC		(1u <<  0) /* Current interrupt enable */
#define COP0_STATUS_KUC		(1u <<  1) /* Current privilege level */
#define COP0_STATUS_IEP		(1u <<  2) /* Previous interrupt enable */
#define COP0_STATUS_KUP		(1u <<  3) /* Previous privilege level */
#define COP0_STATUS_IEO		(1u <<  4) /* Old interrupt enable */
#define COP0_STATUS_KUO		(1u <<  5) /* Old privilege level */
#define COP0_STATUS_IM0		(1u <<  8) /* IRQ mask 0 (software interrupt) */
#define COP0_STATUS_IM1		(1u <<  9) /* IRQ mask 1 (software interrupt) */
#define COP0_STATUS_IM2		(1u << 10) /* IRQ mask 2 (hardware interrupt) */
#define COP0_STATUS_ISC		(1u << 16) /* Isolate cache */
#define COP0_STATUS_BEV		(1u << 22) /* Boot exception vector location */
#define COP0_STATUS_CU0		(1u << 28) /* Coprocessor 0 privilege level */
#define COP0_STATUS_CU2		(1u << 30) /* Coprocessor 2 enable */
typedef uint32_t cop0_status_flag_t;

#define COP0_CAUSE_EXC_BITMASK	(31u <<  2)
#define COP0_CAUSE_EXC_INT	( 0u <<  2) /* Interrupt */
#define COP0_CAUSE_EXC_ADEL	( 4u <<  2) /* Load address error */
#define COP0_CAUSE_EXC_ADES	( 5u <<  2) /* Store address error */
#define COP0_CAUSE_EXC_IBE	( 6u <<  2) /* Instruction bus error */
#define COP0_CAUSE_EXC_DBE	( 7u <<  2) /* Data bus error */
#define COP0_CAUSE_EXC_SYS	( 8u <<  2) /* Syscall */
#define COP0_CAUSE_EXC_BP	( 9u <<  2) /* Breakpoint or break instruction */
#define COP0_CAUSE_EXC_RI	(10u <<  2) /* Reserved instruction */
#define COP0_CAUSE_EXC_CPU	(11u <<  2) /* Coprocessor unusable */
#define COP0_CAUSE_EXC_Ov	(12u <<  2) /* Arithmetic overflow */
#define COP0_CAUSE_IP0		( 1u <<  8) /* IRQ 0 pending (software interrupt) */
#define COP0_CAUSE_IP1		( 1u <<  9) /* IRQ 1 pending (software interrupt) */
#define COP0_CAUSE_IP2		( 1u << 10) /* IRQ 2 pending (hardware interrupt) */
#define COP0_CAUSE_CE_BITMASK	( 3u << 28)
#define COP0_CAUSE_BD		( 1u << 30) /* Exception occurred in delay slot */
typedef uint32_t cop0_cause_flag_t;

/*
 * Note that reg must be a constant value known at compile time, as the
 * mtc0/mfc0 instructions only support addressing coprocessor registers directly
 * through immediates.
 */
DEF(void)
cop0_set_reg(
	const cop0_reg_t reg,
	uint32_t value
	)
{
	__asm__ __volatile__("mtc0 %0, $%1\n" :: "r"(value), "i"(reg));
}

DEF(uint32_t)
cop0_get_reg(const cop0_reg_t reg)
{
	uint32_t value;

	__asm__ __volatile__("mfc0 %0, $%1\n" : "=r"(value) : "i"(reg));
	return (value);
}

DEF(void)
cop0_enable_interrupts(void)
{
	uint32_t status = cop0_get_reg(COP0_STATUS);

	cop0_set_reg(COP0_STATUS, status | COP0_STATUS_IEC | COP0_STATUS_IM2);
}

DEF(uint32_t)
cop0_disable_interrupts(void)
{
	uint32_t status = cop0_get_reg(COP0_STATUS);

	cop0_set_reg(COP0_STATUS, status & ~COP0_STATUS_IEC);
	return (status & COP0_STATUS_IEC);
}

#endif /* _SYS__COP0_H */
