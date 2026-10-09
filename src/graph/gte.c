/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/cop0.h>
#include <sys/gte.h>

#include <graph/gte.h>
#include <graph/gpu.h>

#include <stddef.h>
#include <stdint.h>

void
graph_init_gte(
	uint32_t width,
	uint32_t height
	)
{
	cop0_set_reg(COP0_STATUS, cop0_get_reg(COP0_STATUS) | COP0_STATUS_CU2);

	gte_set_control_reg(GTE_OFX, (width  << 16) / 2);
	gte_set_control_reg(GTE_OFY, (height << 16) / 2);

	int focal = ((width < height) ? width : height);

	gte_set_control_reg(GTE_H, focal / 2);
	gte_set_control_reg(GTE_ZSF3, GRAPH_DMA_CHAIN_OT_SIZE / 3);
	gte_set_control_reg(GTE_ZSF4, GRAPH_DMA_CHAIN_OT_SIZE / 4);
}
