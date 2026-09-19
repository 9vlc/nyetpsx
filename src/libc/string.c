/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

void *
memset(
	void *dest,
	int ch,
	size_t count
	)
{
	uint8_t *_dest = (uint8_t *)dest;

	for (; count; count--) {
		*(_dest++) = (uint8_t)ch;
	}

	return (dest);
}

void *
memcpy(
	void * restrict dest,
	const void * restrict src,
	size_t count
	)
{
	uint8_t *_dest = (uint8_t *)dest;
	const uint8_t *_src = (const uint8_t *)src;

	for (; count; count--) {
		*(_dest++) = *(_src++);
	}

	return (dest);
}

void *
memccpy(
	void * restrict dest,
	const void * restrict src,
	int ch,
	size_t count
	)
{
	uint8_t *_dest = (uint8_t *)dest;
	const uint8_t *_src = (const uint8_t *)src;

	for (; count; count--) {
		uint8_t a = *(_src++);

		*(_dest++) = a;
		if (a == ch) {
			return ((void *)_dest);
		}
	}

	return (0);
}

void *
memmove(
	void *dest,
	const void *src,
	size_t count
	)
{
	uint8_t *_dest = (uint8_t *)dest;
	const uint8_t *_src = (const uint8_t *)src;

	if (_dest == _src) {
		return (dest);
	} else if ((_dest >= &_src[count]) || (&_dest[count] <= _src)) {
		return (memcpy(dest, src, count));
	}

	/* Copy forwards */
	if (_dest < _src) {
		for (; count; count--) {
			*(_dest++) = *(_src++);
		}

	/* Copy backwards */
	} else {
		_src  += count;
		_dest += count;

		for (; count; count--) {
			*(--_dest) = *(--_src);
		}
	}

	return (dest);
}

int
memcmp(
	const void *lhs,
	const void *rhs,
	size_t count
	)
{
	const uint8_t *_lhs = (const uint8_t *)lhs;
	const uint8_t *_rhs = (const uint8_t *)rhs;

	for (; count; count--) {
		uint8_t a = *(_lhs++),
			b = *(_rhs++);

		if (a != b) {
			return (((int) a) - ((int) b));
		}
	}

	return (0);
}

void *
memchr(
	const void *ptr,
	int ch,
	size_t count
	)
{
	const uint8_t *_ptr = (const uint8_t *)ptr;

	for (; count; count--, _ptr++) {
		if (*_ptr == ch) {
			return ((void *)_ptr);
		}
	}

	return (0);
}

/* String manipulation */

char *
strcpy(
	char * restrict dest,
	const char * restrict src
	)
{
	char *_dest = dest;

	while (*src) {
		*(_dest++) = *(src++);
	}

	*_dest = 0;

	return (dest);
}

char *
strncpy(
	char * restrict dest,
	const char * restrict src,
	size_t count
	)
{
	char *_dest = dest;

	for (; count && *src; count--) {
		*(_dest++) = *(src++);
	}

	for (; count; count--) {
		*(_dest++) = 0;
	}

	return (dest);
}

int
strcmp(
	const char *lhs,
	const char *rhs
	)
{
	while (1) {
		char a = *(lhs++),
		     b = *(rhs++);

		if (a != b) {
			return (((int)a) - ((int)b));
		} else if (!a && !b) {
			return (0);
		}
	}
}

int
strncmp(const char *lhs, const char *rhs, size_t count)
{
	for (; count && *lhs && *rhs; count--) {
		char a = *(lhs++),
		     b = *(rhs++);

		if (a != b) {
			return (((int)a) - ((int)b));
		}
	}

	return (0);
}

char *
strchr(const char *str, int ch)
{
	for (; *str; str++) {
		if (*str == ch) {
			return ((char *)str);
		}
	}

	return (0);
}

char *
strrchr(const char *str, int ch)
{
	size_t length = strlen(str);

	for (str += length; length; length--) {
		str--;
		if (*str == ch) {
			return ((char *)str);
		}
	}

	return (0);
}

char *
strpbrk(const char *str, const char *breakSet)
{
	for (; *str; str++) {
		char a = *str;

		for (const char *ch = breakSet; *ch; ch++) {
			if (a == *ch) {
				return ((char *)str);
			}
		}
	}

	return (0);
}

char *
strstr(const char *str, const char *substr)
{
	size_t length = strlen(substr);

	if (!length) {
		return ((char *)str);
	}

	for (; *str; str++) {
		if (!memcmp(str, substr, length)) {
			return ((char *)str);
		}
	}

	return (0);
}

size_t
strlen(const char *str)
{
	size_t length = 0;

	for (; *str; str++) {
		length++;
	}

	return (length);
}


char *
strcat(char * restrict dest, const char * restrict src)
{
	char *_dest = &dest[strlen(dest)];

	while (*src) {
		*(_dest++) = *(src++);
	}

	*_dest = 0;
	return (dest);
}

char *
strncat(char * restrict dest, const char * restrict src, size_t count)
{
	char *_dest = &dest[strlen(dest)];

	for (; count && *src; count--) {
		*(_dest++) = *(src++);
	}

	*_dest = 0;
	return (dest);
}
