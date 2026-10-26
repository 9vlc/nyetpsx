/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/dma.h>
#include <psx/gpu.h>
#include <psx/irq.h>

#include <stdbool.h>

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
	bool interlace_odd
	)
{
	while (!(IRQ_STAT & (1 << IRQ_VSYNC))) {
		__asm__ __volatile__("");
	}

	IRQ_STAT = ~(1 << IRQ_VSYNC);

	if (GPU_GP1 & GP1_STAT_FB_INTERLACE) {
		while (1) {
			uint32_t stat = GPU_GP1;
			uint32_t draw_field = (stat / GP1_STAT_DRAW_FIELD_ODD) & 1;
			uint32_t disp_field = (stat / GP1_STAT_DISP_FIELD_ODD) & 1;

			if (draw_field == disp_field) {
				continue;
			} else if (draw_field == interlace_odd) {
				break;
			}
		}
	}
}
