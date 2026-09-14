/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__MMIO_H
#define _SYS__MMIO_H

#include <stdint.h>

#define ADDR8(A)	((volatile uint8_t *)(A))
#define ADDR16(A)	((volatile uint16_t *)(A))
#define ADDR32(A)	((volatile uint32_t *)(A))
#define MMIO8(A)	(*ADDR8(A))
#define MMIO16(A)	(*ADDR16(A))
#define MMIO32(A)	(*ADDR32(A))

#define RAM_BASE	0x80000000
#define DEV0_BASE	0xBF000000
#define CACHE_BASE	0x9F800000 /* Cannot be accessed from KSEG1 */
#define IO_BASE		0xBF801000
#define DEV8_BASE	0xBF802000
#define DEV1_BASE	0xBFA00000
#define DEV2_BASE	0xBFC00000
#define CPU_BASE	0xFFFE0000

#endif /* _SYS__MMIO_H */
