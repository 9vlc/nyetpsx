/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stddef.h>
#include <stdint.h>
#include <crc32.h>

uint32_t
crc32(
	const char *data,
	const size_t data_len
	)
{
	uint32_t crc = UINT32_MAX;

	for (size_t i = 0; i < data_len; i++) {
		crc = crc32_step(crc, data[i]);
	}

	return (crc ^ UINT32_MAX);
}
