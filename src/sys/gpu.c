/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/dma.h>
#include <sys/gpu.h>
#include <sys/irq.h>

void
gpu_await_dma(
	void
	)
{
	while (DMA_CHCR(DMA_GPU) & DMA_CHCR_ENABLE) {
		__asm__ __volatile__("");
	}

	while (!(GPU_GP1 & GP1_STAT_WRITE_READY)) {
		__asm__ __volatile__("");
	}
}

void
gpu_await_cmd(
	void
	)
{
	while (!(GPU_GP1 & GP1_STAT_CMD_READY)) {
		__asm__ __volatile__("");
	}
}

void
gpu_await_vsync(
	void
	)
{
	while (!(IRQ_STAT & (1 << IRQ_VSYNC))) {
		__asm__ __volatile__("");
	}

	IRQ_STAT = ~(1 << IRQ_VSYNC);
}
