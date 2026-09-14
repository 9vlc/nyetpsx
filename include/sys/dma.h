/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__DMA_H
#define _SYS__DMA_H

#include <sys/mmio.h>

#define DMA_MDEC_IN			0
#define DMA_MDEC_OUT			1
#define DMA_GPU				2
#define DMA_CDROM			3
#define DMA_SPU				4
#define DMA_PIO				5
#define DMA_OTC				6

#define DMA_CHCR_READ			(0u <<  0)
#define DMA_CHCR_WRITE			(1u <<  0)
#define DMA_CHCR_REVERSE		(1u <<  1)
#define DMA_CHCR_CHOPPING		(1u <<  8)
#define DMA_CHCR_MODE_BITMASK		(3u <<  9)
#define DMA_CHCR_MODE_BURST		(0u <<  9)
#define DMA_CHCR_MODE_SLICE		(1u <<  9)
#define DMA_CHCR_MODE_LIST		(2u <<  9)
#define DMA_CHCR_DMA_TIME_BITMASK	(7u << 16)
#define DMA_CHCR_CPU_TIME_BITMASK	(7u << 20)
#define DMA_CHCR_ENABLE			(1u << 24)
#define DMA_CHCR_TRIGGER		(1u << 28)
#define DMA_CHCR_PAUSE			(1u << 29) /* Burst mode only */

#define DMA_DPCR_CH_PRIORITY_BITMASK(N)	(7u << (4u * (N)))
#define DMA_DPCR_CH_PRIORITY(N, PRIO)	(((PRIO) & 7u) << (4u * (N)))
#define DMA_DPCR_CH_ENABLE(N)		((1u << 3) << (4u * (N)))

#define DMA_DICR_CH_MODE_BITMASK	(0x7Fu <<  0)
#define DMA_DICR_BUS_ERROR		(1u << 15)
#define DMA_DICR_CH_ENABLE_BITMASK	(0x7Fu << 16)
#define DMA_DICR_IRQ_ENABLE		(1u << 23)
#define DMA_DICR_CH_STAT_BITMASK	(0x7Fu << 24)
#define DMA_DICR_IRQ			(1u << 31)

#define DMA_DICR_CH_MODE(N)		(1u << ((N) + 0))
#define DMA_DICR_CH_ENABLE(N)		(1u << ((N) + 16))
#define DMA_DICR_CH_STAT(N)		(1u << ((N) + 24))

#define DMA_MADR(N)			MMIO32((IO_BASE | 0x080) + (16u * (N)))
#define DMA_BCR(N)			MMIO32((IO_BASE | 0x084) + (16u * (N)))
#define DMA_CHCR(N)			MMIO32((IO_BASE | 0x088) + (16u * (N)))

#define DMA_DPCR			MMIO32(IO_BASE | 0x0F0)
#define DMA_DICR			MMIO32(IO_BASE | 0x0F4)

#endif /* _SYS__DMA_H */
