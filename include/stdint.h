/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _STDINT_H
#define _STDINT_H

#ifdef __INT8_TYPE__
#  ifndef _INT8_DEFINED
#    define _INT8_DEFINED
typedef __INT8_TYPE__		int8_t;
typedef __UINT8_TYPE__		uint8_t;

#    define INT8_C(v)		__INT8_C(v)
#    define UINT8_C(v)		__UINT8_C(v)

#    define INT8_MIN		(-__INT8_MAX__ - 1)
#    define INT8_MAX		__INT8_MAX__
#    define UINT8_MAX		__UINT8_MAX__
#  endif
#endif

#ifdef __INT16_TYPE__
#  ifndef _INT16_DEFINED
#    define _INT16_DEFINED
typedef __INT16_TYPE__		int16_t;
typedef __UINT16_TYPE__		uint16_t;

#    define INT16_C(v)		__INT16_C(v)
#    define UINT16_C(v)		__UINT16_C(v)

#    define INT16_MIN		(-__INT16_MAX__ - 1)
#    define INT16_MAX		__INT16_MAX__
#    define UINT16_MAX		__UINT16_MAX__
#  endif
#endif

#ifdef __INT32_TYPE__
#  ifndef _INT32_DEFINED
#    define _INT32_DEFINED
typedef __INT32_TYPE__		int32_t;
typedef __UINT32_TYPE__		uint32_t;

#    define INT32_C(v)		__INT32_C(v)
#    define UINT32_C(v)		__UINT32_C(v)

#    define INT32_MIN		(-__INT32_MAX__ - 1)
#    define INT32_MAX		__INT32_MAX__
#    define UINT32_MAX		__UINT32_MAX__
#  endif
#endif

#ifdef __INT64_TYPE__
#  ifndef _INT64_DEFINED
#    define _INT64_DEFINED
typedef __INT64_TYPE__		int64_t;
typedef __UINT64_TYPE__		uint64_t;

#    define INT64_C(v)		__INT64_C(v)
#    define UINT64_C(v)		__UINT64_C(v)

#    define INT64_MIN		(-__INT64_MAX__ - 1)
#    define INT64_MAX		__INT64_MAX__
#    define UINT64_MAX		__UINT64_MAX__
#  endif
#endif

#ifndef _INT_LEAST8_DEFINED
#  define _INT_LEAST8_DEFINED
typedef __INT_LEAST8_TYPE__	int_least8_t;
typedef __UINT_LEAST8_TYPE__	uint_least8_t;

#  define INT_LEAST8_MIN	(-__INT_LEAST8_MAX__ - 1)
#  define INT_LEAST8_MAX	__INT_LEAST8_MAX__
#  define UINT_LEAST8_MAX	__UINT_LEAST8_MAX__
#endif

#ifndef _INT_LEAST16_DEFINED
#  define _INT_LEAST16_DEFINED
typedef __INT_LEAST16_TYPE__	int_least16_t;
typedef __UINT_LEAST16_TYPE__	uint_least16_t;

#  define INT_LEAST16_MIN	(-__INT_LEAST16_MAX__ - 1)
#  define INT_LEAST16_MAX	__INT_LEAST16_MAX__
#  define UINT_LEAST16_MAX	__UINT_LEAST16_MAX__
#endif

#ifndef _INT_LEAST32_DEFINED
#  define _INT_LEAST32_DEFINED
typedef __INT_LEAST32_TYPE__	int_least32_t;
typedef __UINT_LEAST32_TYPE__	uint_least32_t;

#  define INT_LEAST32_MIN	(-__INT_LEAST32_MAX__ - 1)
#  define INT_LEAST32_MAX	__INT_LEAST32_MAX__
#  define UINT_LEAST32_MAX	__UINT_LEAST32_MAX__
#endif

#ifndef _INT_LEAST64_DEFINED
#  define _INT_LEAST64_DEFINED
typedef __INT_LEAST64_TYPE__	int_least64_t;
typedef __UINT_LEAST64_TYPE__	uint_least64_t;

#  define INT_LEAST64_MIN	(-__INT_LEAST64_MAX__ - 1)
#  define INT_LEAST64_MAX	__INT_LEAST64_MAX__
#  define UINT_LEAST64_MAX	__UINT_LEAST64_MAX__
#endif

#ifndef _INT_FAST8_DEFINED
#  define _INT_FAST8_DEFINED
typedef __INT_FAST8_TYPE__	int_fast8_t;
typedef __UINT_FAST8_TYPE__	uint_fast8_t;

#  define INT_FAST8_MIN		(-__INT_FAST8_MAX__ - 1)
#  define INT_FAST8_MAX		__INT_FAST8_MAX__
#  define UINT_FAST8_MAX	__UINT_FAST8_MAX__
#endif

#ifndef _INT_FAST16_DEFINED
#  define _INT_FAST16_DEFINED
typedef __INT_FAST16_TYPE__	int_fast16_t;
typedef __UINT_FAST16_TYPE__	uint_fast16_t;

#  define INT_FAST16_MIN	(-__INT_FAST16_MAX__ - 1)
#  define INT_FAST16_MAX	__INT_FAST16_MAX__
#  define UINT_FAST16_MAX	__UINT_FAST16_MAX__
#endif

#ifndef _INT_FAST32_DEFINED
#  define _INT_FAST32_DEFINED
typedef __INT_FAST32_TYPE__	int_fast32_t;
typedef __UINT_FAST32_TYPE__	uint_fast32_t;

#  define INT_FAST32_MIN	(-__INT_FAST32_MAX__ - 1)
#  define INT_FAST32_MAX	__INT_FAST32_MAX__
#  define UINT_FAST32_MAX	__UINT_FAST32_MAX__
#endif

#ifndef _INT_FAST64_DEFINED
#  define _INT_FAST64_DEFINED
typedef __INT_FAST64_TYPE__	int_fast64_t;
typedef __UINT_FAST64_TYPE__	uint_fast64_t;

#  define INT_FAST64_MIN	(-__INT_FAST64_MAX__ - 1)
#  define INT_FAST64_MAX	__INT_FAST64_MAX__
#  define UINT_FAST64_MAX	__UINT_FAST64_MAX__
#endif

#ifndef _INTPTR_DEFINED
#  define _INTPTR_DEFINED
typedef __INTPTR_TYPE__		intptr_t;
typedef __UINTPTR_TYPE__	uintptr_t;

#  define INTPTR_MIN		(-__INTPTR_MAX__ - 1)
#  define INTPTR_MAX		__INTPTR_MAX__
#  define UINTPTR_MAX		__UINTPTR_MAX__
#endif

#ifndef _INTMAX_DEFINED
#  define _INTMAX_DEFINED
typedef __INTMAX_TYPE__		intmax_t;
typedef __UINTMAX_TYPE__	uintmax_t;

#  define INTMAX_C(v)		__INTMAX_C(v)
#  define UINTMAX_C(v)		__UINTMAX_C(v)

#  define INTMAX_MIN		(-__INTMAX_MAX__ - 1)
#  define INTMAX_MAX		__INTMAX_MAX__
#  define UINTMAX_MAX		__UINTMAX_MAX__
#endif

#endif /* _STDINT_H */
