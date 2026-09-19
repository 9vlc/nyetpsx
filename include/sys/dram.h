/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__DRAM_H
#define _SYS__DRAM_H

#include <sys/mmio.h>

/*
 * DRAM controller register flag definitions
 */

enum dram_control_flag {
	DRAM_CTRL_UNKNOWN1		= 1u << 3, /* Crashes when zero */
	DRAM_CTRL_REFRESH_BITMASK	= 3u << 4,
	DRAM_CTRL_REFRESH_256		= 0u << 4,
	DRAM_CTRL_REFRESH_320		= 1u << 4,
	DRAM_CTRL_REFRESH_384		= 2u << 4,
	DRAM_CTRL_REFRESH_448		= 3u << 4,
	DRAM_CTRL_FETCH_DELAY		= 1u << 7, /* Delay on code+data fetch */
	DRAM_CTRL_UNKNOWN2		= 1u << 8,
	DRAM_CTRL_SIZE_BITMASK		= (1u << 9) | (1u << 11),
	DRAM_CTRL_SIZE_1MB		= (0u << 9) | (0u << 11),
	DRAM_CTRL_SIZE_2MB		= (0u << 9) | (1u << 11),
	DRAM_CTRL_SIZE_4MB		= (1u << 9) | (0u << 11),
	DRAM_CTRL_SIZE_8MB		= (1u << 9) | (1u << 11),
	DRAM_CTRL_BANKS_BITMASK		= 1u << 10,
	DRAM_CTRL_BANKS_1		= 0u << 10, /* Disable /RAS1 bank */
	DRAM_CTRL_BANKS_2		= 1u << 10, /* Enable /RAS1 bank */
};
typedef enum dram_control_flag dram_control_flag_t;

#define DRAM_CTRL			MMIO32(IO_BASE | 0x060)

#endif /* _SYS__DRAM_H */
