/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _TRIG_H
#define _TRIG_H

#define DEF(type) static inline type __attribute__((always_inline))

#define ISIN_SHIFT 10

int isin(int x);

DEF(int)
icos(int x)
{
	return (isin(x + (1 << ISIN_SHIFT)));
}

#undef DEF

#endif /* _TRIG_H */
