/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _PSX__CPU_H
#define _PSX__CPU_H

#include <psx/mmio.h>

#include <stdint.h>

/*
 * CPU BIU/cache configuration register flag definitions
 *
 * The LOCK, INV, TAG and RAM bits only take effect while the cache is isolated,
 * ie. while bit 16 of the COP0 status register is set. The cache is also
 * configured by the BIOS and normally does not need to be changed at runtime.
 */

#define CPU_BCC_LOCK		(1u << 0) /* Enable cache lock mode */
#define CPU_BCC_INV		(1u << 1) /* Enable cache invalidation mode */
#define CPU_BCC_TAG		(1u << 2) /* Enable cache tag test mode */
#define CPU_BCC_RAM		(1u << 3) /* Enable scratchpad mode */
#define CPU_BCC_DBLKSZ_BITMASK	(3u << 4)
#define CPU_BCC_DBLKSZ_2	(0u << 4) /* 2 word data cache refill */
#define CPU_BCC_DBLKSZ_4	(1u << 4) /* 4 word data cache refill */
#define CPU_BCC_DS		(1u << 7) /* Enable data cache (disables scratchpad) */
#define CPU_BCC_IBLKSZ_BITMASK	(3u << 8)
#define CPU_BCC_IBLKSZ_2	(0u << 8) /* 2 word instruction cache refill */
#define CPU_BCC_IBLKSZ_4	(1u << 8) /* 4 word instruction cache refill */
#define CPU_BCC_IS0		(1u << 10) /* Read-only, always zero */
#define CPU_BCC_IS1		(1u << 11) /* Enable instruction cache */
#define CPU_BCC_INTP		(1u << 12)
#define CPU_BCC_RDPRI		(1u << 13)
#define CPU_BCC_NOPAD		(1u << 14)
#define CPU_BCC_BGNT		(1u << 15)
#define CPU_BCC_LDSCH		(1u << 16)
#define CPU_BCC_NOSTR		(1u << 17)
typedef uint32_t cpu_bcc_flag_t;

#define CPU_BCC		MMIO32(CPU_BASE | 0x130)

void cpu_flush_cache(void);

#endif /* _PSX__CPU_H */
