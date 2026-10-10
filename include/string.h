/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

void *
memset(
	void *dest,
	int ch,
	size_t count
	);

void *
memcpy(
	void * restrict dest,
	const void * restrict src,
	size_t count
	);

void *
memccpy(
	void * restrict dest,
	const void * restrict src,
	int ch,
	size_t count
	);

void *
memmove(
	void *dest,
	const void *src,
	size_t count
	);

int
memcmp(
	const void *lhs,
	const void *rhs,
	size_t count
	);

void *
memchr(
	const void *ptr,
	int ch,
	size_t count
	);

char *
strcpy(
	char * restrict dest,
	const char * restrict src
	);

char *
strncpy(
	char * restrict dest,
	const char * restrict src,
	size_t count
	);

int
strcmp(
	const char *lhs,
	const char *rhs
	);

int
strncmp(
	const char *lhs,
	const char *rhs,
	size_t count
	);

char *
strchr(
	const char *str,
	int ch
	);

char *
strrchr(
	const char *str,
	int ch
	);

char *
strpbrk(
	const char *str,
	const char *breakSet
	);

char *
strstr(
	const char *str,
	const char *substr
	);

size_t strlen(const char *str);

char *
strcat(
	char * restrict dest,
	const char * restrict src
	);

char *
strncat(
	char * restrict dest,
	const char * restrict src,
	size_t count
	);

#endif /* _STRING_H */
