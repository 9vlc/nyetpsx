/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _PSX__DRAM_H
#define _PSX__DRAM_H

#include <psx/mmio.h>

#include <stdint.h>

/*
 * DRAM controller register flag definitions
 */

#define DRAM_CTRL_REF_TIMEOUT_BITMASK	(7 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_0		(0 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_4		(1 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_8		(2 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_16	(3 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_32	(4 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_64	(5 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_128	(6 <<  0)
#define DRAM_CTRL_REF_TIMEOUT_INF	(7 <<  0)
#define DRAM_CTRL_BYTE_CAS		(1 <<  3)
#define DRAM_CTRL_REF_PERIOD_BITMASK	(3 <<  4)
#define DRAM_CTRL_REF_PERIOD_256	(0 <<  4)
#define DRAM_CTRL_REF_PERIOD_320	(1 <<  4)
#define DRAM_CTRL_REF_PERIOD_384	(2 <<  4)
#define DRAM_CTRL_REF_PERIOD_448	(3 <<  4)
#define DRAM_CTRL_UNKNOWN		(1 <<  6)
#define DRAM_CTRL_ACCESS_DELAY		(1 <<  7)
#define DRAM_CTRL_SIZE_BITMASK		((3 << 8) | (1 << 11))
#define DRAM_CTRL_SIZE_1MB		((0 << 8) | (0 << 11))
#define DRAM_CTRL_SIZE_1MB_4MB		((1 << 8) | (0 << 11))
#define DRAM_CTRL_SIZE_4MB_1MB		((2 << 8) | (0 << 11))
#define DRAM_CTRL_SIZE_4MB		((3 << 8) | (0 << 11))
#define DRAM_CTRL_SIZE_2MB		((0 << 8) | (1 << 11))
#define DRAM_CTRL_SIZE_2MB_8MB		((1 << 8) | (1 << 11))
#define DRAM_CTRL_SIZE_8MB_2MB		((2 << 8) | (1 << 11))
#define DRAM_CTRL_SIZE_8MB		((3 << 8) | (1 << 11))
#define DRAM_CTRL_BANKS_BITMASK		(0 << 10)
#define DRAM_CTRL_BANKS_1		(0 << 10) /* Disable RAS1 bank */
#define DRAM_CTRL_BANKS_2		(1 << 10) /* Enable RAS1 bank */
typedef uint32_t dram_control_flag_t;

#define DRAM_CTRL			MMIO32(IO_BASE | 0x060)

#endif /* _PSX__DRAM_H */
