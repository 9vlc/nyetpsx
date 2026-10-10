/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__DELAY_H
#define _SYS__DELAY_H

#include <sys/cpu.h>
#include <sys/freq.h>

#include <stdint.h>

#define DEF(T) static inline T __attribute__((always_inline))
#define DIV(N, D) (((N) + (D) / 2) / (D))

DEF(void)
delay_cycles(
	uint32_t time
	)
{
	__asm__ volatile(
		".set push\n"
		".set noreorder\n"
		"bgtz  %0, .\n"
		"addiu %0, -2\n"
		".set pop\n"
		: "+r"(time)
	);
}

DEF(void)
delay_us(
	uint32_t time
	)
{
	delay_cycles(DIV(time * DIV(F_CPU * 8, 1000000), 8));
}

#undef DEF
#undef DIV

#endif /* _SYS__DELAY_H */
