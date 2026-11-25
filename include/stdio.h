/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _STDIO_H
#define _STDIO_H

/*
 * Not actually standard (tm)
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#if STDIO_USE_SIO1 == 1
#  include <psx/sio.h>
#  define putchar	sio1_putchar
#  define getchar	sio1_getchar
#else
#  include <sys/syscall.h>
#  define putchar	sys_putchar
#  define getchar	sys_getchar
#endif

void putx8(uint8_t x);
void putx16(uint16_t x);
void putx32(uint32_t x);

int
uvprintf(
	const char *fmt,
	va_list ap
	);

int
uprintf(
	const char *fmt,
	...
	);

int
uvcprintf(
	int (*putchar)(int c),
	const char *fmt,
	va_list ap
	);

int
ucprintf(
	int (*putchar)(int c),
	const char *fmt,
	...
	);

int
uvsprintf(
	char *str,
	const char *fmt,
	va_list ap
	);

int
usprintf(
	char *str,
	const char *fmt,
	...
	);

int
uvsnprintf(
	char *str,
	size_t size,
	const char *fmt,
	va_list ap
	);

int
usnprintf(
	char *str,
	size_t size,
	const char *fmt,
	...
	);

#define vprintf		uvprintf
#define printf		uprintf
#define vcprintf	uvcprintf
#define cprintf		ucprintf
#define vsprintf	uvsprintf
#define sprintf		usprintf
#define vsnprintf	uvsnprintf
#define snprintf	usnprintf

#endif /* _STDIO_H */
