/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sio.h>

void
_assertAbort(const char *file, int line, const char *expr) {
#ifndef NDEBUG
	puts(file);
	putc(':');
	putu32(line);
	puts(" assert(");
	puts(expr);
	putc(')');
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}

void
abort(void)
{
#ifndef NDEBUG
	puts("abort()");
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}

void
__cxa_pure_virtual(void)
{
#ifndef NDEBUG
	puts("__cxa_pure_virtual()");
#endif

	while (1) {
		__asm__ __volatile__("");
	}
}
