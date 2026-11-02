/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 *
 * https://github.com/glitchub/arith64/blob/master/arith64.c
 */

#include <stddef.h>
#include <stdint.h>

/* Needed for functions here that depend on external ASM implementations */
#include "arith64.h"

union arith64_word {
	struct { uint32_t lo, hi; } u32;
	struct { int32_t  lo, hi; } s32;
	uint64_t u64;
	int64_t  s64;
};
typedef union arith64_word arith64_word_t;

#define ARITH64_HI(N)		(arith64_word_t){.u64=N}.u32.hi
#define ARITH64_LO(N)		(arith64_word_t){.u64=N}.u32.lo

#define ARITH64_NEGATE(A, B)	(((A) ^ ((((int64_t)(B)) >= 0) - 1)) + (((int64_t)(B)) < 0))
#define ARITH64_ABS(N)		ARITH64_NEGATE(N, N)

int64_t
__absvdi2(
	int64_t a
	)
{
	return (ARITH64_ABS(a));
}

int64_t
__ashldi3(
	int64_t a,
	int b
	)
{
	arith64_word_t w = {.s64 = a};

	b &= 63;
	if (b >= 32) {
		w.u32.hi = w.u32.lo << (b - 32);
		w.u32.lo = 0;
	} else if (b) {
		w.u32.hi = (w.u32.lo >> (32 - b)) | (w.u32.hi << b);
		w.u32.lo <<= b;
	}

	return (w.s64);
}

int64_t
__ashrdi3(
	int64_t a,
	int b
	)
{
	arith64_word_t w = {.s64 = a};

	b &= 63;
	if (b >= 32) {
		w.s32.lo = w.s32.hi >> (b - 32);
		w.s32.hi >>= 31;
	} else if (b) {
		w.u32.lo = (w.u32.hi << (32 - b)) | (w.u32.lo >> b);
		w.s32.hi >>= b;
	}

	return (w.s64);
}

/*
 * Already implemented in arith64_clz.s
 */

/*
int
__clzsi2(
	uint32_t a
	)
{
	int b, n = 0;

	b = !(a & 0xFFFF0000) << 4; n += b; a <<= b;
	b = !(a & 0xFF000000) << 3; n += b; a <<= b;
	b = !(a & 0xF0000000) << 2; n += b; a <<= b;
	b = !(a & 0xC0000000) << 1; n += b; a <<= b;

	return (n + !(a & 0x80000000));
}

int
__clzdi2(
	uint64_t a
	)
{
	int b, n = 0;

	b = !(a & 0xFFFFFFFF00000000ULL) << 5; n += b; a <<= b;
	b = !(a & 0xFFFF000000000000ULL) << 4; n += b; a <<= b;
	b = !(a & 0xFF00000000000000ULL) << 3; n += b; a <<= b;
	b = !(a & 0xF000000000000000ULL) << 2; n += b; a <<= b;
	b = !(a & 0xC000000000000000ULL) << 1; n += b; a <<= b;

	return (n + !(a & 0x8000000000000000ULL));
}
*/

int
__ctzsi2(
	uint32_t a
	)
{
	int b, n = 0;

	b = !(a & 0x0000FFFF) << 4; n += b; a >>= b;
	b = !(a & 0x000000FF) << 3; n += b; a >>= b;
	b = !(a & 0x0000000F) << 2; n += b; a >>= b;
	b = !(a & 0x00000003) << 1; n += b; a >>= b;

	return (n + !(a & 0x00000001));
}

int
__ctzdi2(
	uint64_t a
	)
{
	int b, n = 0;

	b = !(a & 0x00000000FFFFFFFFULL) << 5; n += b; a >>= b;
	b = !(a & 0x000000000000FFFFULL) << 4; n += b; a >>= b;
	b = !(a & 0x00000000000000FFULL) << 3; n += b; a >>= b;
	b = !(a & 0x000000000000000FULL) << 2; n += b; a >>= b;
	b = !(a & 0x0000000000000003ULL) << 1; n += b; a >>= b;

	return (n + !(a & 0x0000000000000001ULL));
}

uint64_t
__divmoddi4(
	uint64_t a,
	uint64_t b,
	uint64_t *c
	)
{
	if (b > a) {
		if (c) {
			*c = a;
		}
		return (0);
	}

	if (!ARITH64_HI(b)) {
		if (b == 0) {
			volatile char x = 0;
			x = 1 / x;
		}

		if (b == 1) {
			if (c) {
				*c = 0;
			}
			return (a);
		}

		if (!ARITH64_HI(a)) {
			if (c) {
				*c = ARITH64_LO(a) % ARITH64_LO(b);
			}
			return (ARITH64_LO(a) / ARITH64_LO(b));
		}
	}

	char bits = __clzdi2(b) - __clzdi2(a) + 1;
	uint64_t rem = a >> bits;
	a <<= 64 - bits;

	uint64_t wrap = 0;
	while (bits-- > 0) {
		rem  = (rem << 1) | (a >> 63);
		a    = (a << 1) | (wrap & 1);
		wrap = ((int64_t)(b - rem - 1) >> 63);
		rem -= b & wrap;
	}

	if (c) {
		*c = rem;
	}

	return ((a << 1) | (wrap & 1));
}

int64_t
__divdi3(
	int64_t a,
	int64_t b
	)
{
	uint64_t q = __divmoddi4(ARITH64_ABS(a), ARITH64_ABS(b), (void *)0);

	return (ARITH64_NEGATE(q, a ^ b));
}

int
__ffsdi2(
	uint64_t a
	)
{
	return (a ? __ctzdi2(a) + 1 : 0);
}

uint64_t
__lshrdi3(
	uint64_t a,
	int b
	)
{
	arith64_word_t w = {.u64 = a};

	b &= 63;
	if (b >= 32) {
		w.u32.lo = w.u32.hi >> (b - 32);
		w.u32.hi = 0;
	} else if (b) {
		w.u32.lo = (w.u32.hi << (32 - b)) | (w.u32.lo >> b);
		w.u32.hi >>= b;
	}

	return (w.u64);
}

int64_t
__moddi3(
	int64_t a,
	int64_t b
	)
{
	uint64_t r;
	__divmoddi4(ARITH64_ABS(a), ARITH64_ABS(b), &r);

	return (ARITH64_NEGATE(r, a));
}

int
__popcountsi2(
	uint32_t a
	)
{
	a = a - ((a >> 1) & 0x55555555);
	a = ((a >> 2) & 0x33333333) + (a & 0x33333333);
	a = (a + (a >> 4)) & 0x0F0F0F0F;
	a = (a + (a >> 16));

	return ((a + (a >> 8)) & 63);
}

int
__popcountdi2(
	uint64_t a
	)
{
	a = a - ((a >> 1) & 0x5555555555555555ULL);
	a = ((a >> 2) & 0x3333333333333333ULL) + (a & 0x3333333333333333ULL);
	a = (a + (a >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
	a = (a + (a >> 32));
	a = (a + (a >> 16));

	return ((a + (a >> 8)) & 127);
}

uint64_t
__udivdi3(
	uint64_t a,
	uint64_t b
	)
{
	return (__divmoddi4(a, b, NULL));
}

uint64_t
__umoddi3(
	uint64_t a,
	uint64_t b
	)
{
	uint64_t r;
	__divmoddi4(a, b, &r);

	return (r);
}
