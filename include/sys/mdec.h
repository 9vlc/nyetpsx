/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__MDEC_H
#define _SYS__MDEC_H

#include <sys/mmio.h>

#define MDEC_CMD_LENGTH_BITMASK		(0xFFFFu <<  0) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_USE_CHROMA		(1u <<  0) /* MDEC_CMD_OP_SET_QUANT_TABLE */
#define MDEC_CMD_SIGNED			(1u << 25) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_16BPP_MASK		(1u << 26) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_FORMAT_BITMASK		(3u << 27) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_FORMAT_4BPP		(0u << 27) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_FORMAT_8BPP		(1u << 27) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_FORMAT_24BPP		(2u << 27) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_FORMAT_16BPP		(3u << 27) /* MDEC_CMD_OP_DECODE */
#define MDEC_CMD_OP_BITMASK		(7u << 29)
#define MDEC_CMD_OP_NOP			(0u << 29)
#define MDEC_CMD_OP_DECODE		(1u << 29)
#define MDEC_CMD_OP_SET_QUANT_TABLE	(2u << 29)
#define MDEC_CMD_OP_SET_IDCT_TABLE	(3u << 29)

#define MDEC_STAT_LENGTH_BITMASK	(0xFFFFu <<  0)
#define MDEC_STAT_BLOCK_BITMASK		(7u << 16)
#define MDEC_STAT_BLOCK_Y0		(0u << 16)
#define MDEC_STAT_BLOCK_Y1		(1u << 16)
#define MDEC_STAT_BLOCK_Y2		(2u << 16)
#define MDEC_STAT_BLOCK_Y3		(3u << 16)
#define MDEC_STAT_BLOCK_CR		(4u << 16)
#define MDEC_STAT_BLOCK_CB		(5u << 16)
#define MDEC_STAT_16BPP_MASK		(1u << 23)
#define MDEC_STAT_SIGNED		(1u << 24)
#define MDEC_STAT_FORMAT_BITMASK	(3u << 25)
#define MDEC_STAT_FORMAT_4BPP		(0u << 25)
#define MDEC_STAT_FORMAT_8BPP		(1u << 25)
#define MDEC_STAT_FORMAT_24BPP		(2u << 25)
#define MDEC_STAT_FORMAT_16BPP		(3u << 25)
#define MDEC_STAT_DREQ_OUT		(1u << 27)
#define MDEC_STAT_DREQ_IN		(1u << 28)
#define MDEC_STAT_BUSY			(1u << 29)
#define MDEC_STAT_DATA_FULL		(1u << 30)
#define MDEC_STAT_DATA_EMPTY		(1u << 31)

#define MDEC_CTRL_DMA_OUT		(1u << 29)
#define MDEC_CTRL_DMA_IN		(1u << 30)
#define MDEC_CTRL_RESET			(1u << 31)

#define MDEC0 MMIO32(IO_BASE | 0x820)
#define MDEC1 MMIO32(IO_BASE | 0x824)

#endif /* _SYS__MDEC_H */
