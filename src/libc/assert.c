/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdio.h>

void
_assert_abort(
	const char *file,
	int line,
	const char *expr
	)
{
#ifndef NDEBUG
	uprintf("%s:%d: assert(%s)\n", file, line, expr);
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}

void
abort(
	void
	)
{
#ifndef NDEBUG
	uprintf("abort\n");
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}

void
__cxa_pure_virtual(
	void
	)
{
#ifndef NDEBUG
	uprintf("__cxa_pure_virtual()\n");
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}
