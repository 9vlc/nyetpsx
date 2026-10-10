/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _ASSERT_H
#define _ASSERT_H

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) \
	((expr) ? ((void)0) : _assert_abort(__FILE__, __LINE__, #expr))
#endif

void
_assert_abort(
	const char *file,
	int line,
	const char *expr
	);

#endif /* _ASSERT_H */
