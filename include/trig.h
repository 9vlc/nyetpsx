/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _TRIG_H
#define _TRIG_H

#define DEF(type) static inline type __attribute__((always_inline))

/* Same as GTE */
#define TRIG_SHIFT	12
#define TRIG_UNIT	(1u << TRIG_SHIFT)

int isin(int x);

DEF(int)
icos(
	int x
	)
{
	return (isin(x + TRIG_UNIT));
}

#undef DEF

#endif /* _TRIG_H */
