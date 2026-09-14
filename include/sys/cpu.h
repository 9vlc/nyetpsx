/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__CPU_H
#define _SYS__CPU_H

#include <sys/mmio.h>

#define CPU_BCC_LOCK		(1u <<  0)
#define CPU_BCC_INV		(1u <<  1)
#define CPU_BCC_TAG		(1u <<  2)
#define CPU_BCC_RAM		(1u <<  3)
#define CPU_BCC_DBLKSZ_BITMASK	(3u <<  4)
#define CPU_BCC_DBLKSZ_2	(0u <<  4)
#define CPU_BCC_DBLKSZ_4	(1u <<  4)
#define CPU_BCC_DS		(1u <<  7)
#define CPU_BCC_IBLKSZ_BITMASK	(3u <<  8)
#define CPU_BCC_IBLKSZ_2	(0u <<  8)
#define CPU_BCC_IBLKSZ_4	(1u <<  8)
#define CPU_BCC_IS0		(1u << 10)
#define CPU_BCC_IS1		(1u << 11)
#define CPU_BCC_INTP		(1u << 12)
#define CPU_BCC_RDPRI		(1u << 13)
#define CPU_BCC_NOPAD		(1u << 14)
#define CPU_BCC_BGNT		(1u << 15)
#define CPU_BCC_LDSCH		(1u << 16)
#define CPU_BCC_NOSTR		(1u << 17)

#define CPU_BCC			MMIO32(CPU_BASE | 0x130)

void cpu_flush_cache(void);

#endif /* _SYS__CPU_H */
