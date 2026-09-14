/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__BUS_H
#define _SYS__BUS_H

#include <sys/mmio.h>

#define BIU_CTRL_WRITE_DELAY_BITMASK	(15u << 0)
#define BIU_CTRL_READ_DELAY_BITMASK	(15u << 4)
#define BIU_CTRL_RECOVERY		(1u << 8)
#define BIU_CTRL_HOLD			(1u << 9)
#define BIU_CTRL_FLOAT			(1u << 10)
#define BIU_CTRL_PRESTROBE		(1u << 11)
#define BIU_CTRL_WIDTH_8		(0u << 12)
#define BIU_CTRL_WIDTH_16		(1u << 12)
#define BIU_CTRL_AUTO_INCR		(1u << 13)
#define BIU_CTRL_SIZE_BITMASK		(31u << 16)
#define BIU_CTRL_DMA_DELAY_BITMASK	(15u << 24)
#define BIU_CTRL_ADDR_ERROR		(1u << 28)
#define BIU_CTRL_DMA_DELAY		(1u << 29)
#define BIU_CTRL_DMA32			(1u << 30)
#define BIU_CTRL_WAIT			(1u << 31)

#define BIU_DEV0_ADDR		MMIO32(IO_BASE | 0x000) /* PIO/arcade */
#define BIU_DEV8_ADDR		MMIO32(IO_BASE | 0x004) /* PIO/debug */
#define BIU_DEV0_CTRL		MMIO32(IO_BASE | 0x008) /* PIO/arcade */
#define BIU_DEV1_CTRL		MMIO32(IO_BASE | 0x00C) /* PIO/arcade/debug */
#define BIU_DEV2_CTRL		MMIO32(IO_BASE | 0x010) /* BIOS ROM */
#define BIU_DEV4_CTRL		MMIO32(IO_BASE | 0x014) /* SPU */
#define BIU_DEV5_CTRL		MMIO32(IO_BASE | 0x018) /* CD-ROM */
#define BIU_DEV8_CTRL		MMIO32(IO_BASE | 0x0CC) /* PIO/debug */
#define BIU_COM_DELAY		MMIO32(IO_BASE | 0x020)

#endif /* _SYS__PSX_H */
