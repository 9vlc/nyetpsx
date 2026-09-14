/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _STDDEF_H
#define _STDDEF_H

#include <stdint.h>

#ifndef _SIZE_T_DEFINED
#  define _SIZE_T_DEFINED
typedef uint32_t		size_t;
typedef int32_t			ssize_t;

#  define SIZE_MAX		UINT32_MAX
#  define SSIZE_MIN		INT32_MIN
#  define SSIZE_MAX		INT32_MAX
#endif

#ifndef NULL
#  define NULL			((void *)0)
#endif

#ifndef offsetof
#  define offsetof(T, F)	__builtin_offsetof(T, F)
#endif

#endif /* _STDDEF_H */
