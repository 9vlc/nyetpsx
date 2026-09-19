/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GPUCTL_H
#define _GRAPH__GPUCTL_H

#include <sys/gpu.h>

#include <stdbool.h>
#include <stdint.h>

#define DEF(type) static inline type __attribute__((always_inline))

DEF(void)
graph_wait_for_gpu(void)
{
	while (!(GPU_GP1 & GP1_STAT_WRITE_READY)) {
		__asm__ __volatile__("");
	}
}


DEF(void)
graph_wait_for_vsync(void)
{
	while (!(IRQ_STAT & (1 << IRQ_VSYNC))) {
		__asm__ volatile("");
	}

	IRQ_STAT = ~(1 << IRQ_VSYNC);
}

#undef DEF

#endif /* _GRAPH__GPUCTL_H */
