/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _ARITH64_H
#define _ARITH64_H

#include <stdint.h>

int64_t
__absvdi2(
	int64_t a
	);

int64_t
__ashldi3(
	int64_t a,
	int b
	);

int64_t
__ashrdi3(
	int64_t a,
	int b
	);

int
__clzsi2(
	uint32_t a
	);

int
__clzdi2(
	uint64_t a
	);

int
__ctzsi2(
	uint32_t a
	);

int
__ctzdi2(
	uint64_t a
	);

uint64_t
__divmoddi4(
	uint64_t a,
	uint64_t b,
	uint64_t *c
	);

int64_t
__divdi3(
	int64_t a,
	int64_t b
	);

int
__ffsdi2(
	uint64_t a
	);

uint64_t
__lshrdi3(
	uint64_t a,
	int b
	);

int64_t
__moddi3(
	int64_t a,
	int64_t b
	);

int
__popcountsi2(
	uint32_t a
	);

int
__popcountdi2(
	uint64_t a
	);

uint64_t
__udivdi3(
	uint64_t a,
	uint64_t b
	);

uint64_t
__umoddi3(
	uint64_t a,
	uint64_t b
	);

#endif /* _ARITH64_H */
