/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _STDARG_H
#define _STDARG_H

#ifndef _VA_LIST_DEFINED
#  define _VA_LIST_DEFINED
typedef __builtin_va_list	va_list;
#endif

#define va_start(AP, LAST)	__builtin_va_start((AP), (LAST))
#define va_arg(AP, TYPE)	__builtin_va_arg((AP), TYPE)
#define va_copy(DEST, SRC)	__builtin_va_copy((DEST), (SRC))
#define va_end(AP)		__builtin_va_end(AP)

#endif /* _STDARG_H */
