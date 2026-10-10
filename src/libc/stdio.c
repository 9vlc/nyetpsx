/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <sys/sio.h>

#include <stdio.h>

static inline void
putxn(uint8_t n)
{
        putc("0123456789ABCDEF"[n & 0xF]);
}

void
putx8(uint8_t x)
{
	for (uint32_t i = 0; i < 8; i += 4) {
		putxn(x >> (8 - 4 - i));
	}
}

void
putx16(uint16_t x)
{
	for (uint32_t i = 0; i < 16; i += 4) {
		putxn(x >> (16 - 4 - i));
	}
}


void
putx32(uint32_t x)
{
	for (uint32_t i = 0; i < 32; i += 4) {
		putxn(x >> (32 - 4 - i));
	}
}
