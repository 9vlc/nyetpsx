/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/dma.h>
#include <psx/gpu.h>
#include <psx/irq.h>

#include <stdbool.h>

bool
gpu_is_dma_ready(
	void
	)
{
	if (DMA_CHCR(DMA_GPU) & DMA_CHCR_ENABLE) {
		return (false);
	}

	if (!(GPU_GP1 & GP1_STAT_WRITE_READY)) {
		return (false);
	}

	return (true);
}

void
gpu_await_dma(
	void
	)
{
	while (!gpu_is_dma_ready()) {
		__asm__ __volatile__("");
	}
}

bool
gpu_is_cmd_ready(
	void
	)
{
	if (!(GPU_GP1 & GP1_STAT_CMD_READY)) {
		return (false);
	}

	return (true);
}

void
gpu_await_cmd(
	void
	)
{
	while (!gpu_is_cmd_ready()) {
		__asm__ __volatile__("");
	}
}

void
gpu_ack_vsync(
	void
	)
{
	IRQ_STAT = ~(1 << IRQ_VSYNC);
}

bool
gpu_is_vsync_ready(
	void
	)
{
	return ((IRQ_STAT & (1 << IRQ_VSYNC)));
}

bool
gpu_is_field_ready(
	bool interlace_odd
	)
{
	uint32_t stat = GPU_GP1;

	if (!(stat & GP1_STAT_FB_INTERLACE)) {
		return (true);
	}

	uint32_t draw_field = (stat / GP1_STAT_DRAW_FIELD_ODD) & 1;
	uint32_t disp_field = (stat / GP1_STAT_DISP_FIELD_ODD) & 1;

	return ((draw_field != disp_field) && (draw_field == interlace_odd));
}

void
gpu_await_vsync(
	bool interlace_odd
	)
{
	while (!gpu_is_vsync_ready()) {
		__asm__ __volatile__("");
	}

	gpu_ack_vsync();

	while (!gpu_is_field_ready(interlace_odd)) {
		__asm__ __volatile__("");
	}
}
