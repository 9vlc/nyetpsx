/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__DMA_H
#define _SYS__DMA_H

#include <sys/mmio.h>

/*
 * DMA channel definitions
 */

enum dma_channel {
	DMA_MDEC_IN	= 0,
	DMA_MDEC_OUT	= 1,
	DMA_GPU		= 2,
	DMA_CDROM	= 3,
	DMA_SPU		= 4,
	DMA_PIO		= 5,
	DMA_OTC		= 6,
};
typedef enum dma_channel dma_channel_t;

/*
 * DMA channel control register flag definitions
 */

enum dma_chcr_flag {
	DMA_CHCR_READ			= 0u <<  0, /* Device to RAM */
	DMA_CHCR_WRITE			= 1u <<  0, /* RAM to device */
	DMA_CHCR_REVERSE		= 1u <<  1, /* Decrement MADR */
	DMA_CHCR_CHOPPING		= 1u <<  8, /* Hangs in slice mode */
	DMA_CHCR_MODE_BITMASK		= 3u <<  9,
	DMA_CHCR_MODE_BURST		= 0u <<  9,
	DMA_CHCR_MODE_SLICE		= 1u <<  9,
	DMA_CHCR_MODE_LIST		= 2u <<  9,
	DMA_CHCR_DMA_WINDOW_BITMASK	= 7u << 16, /* Chop size (1 << N words) */
	DMA_CHCR_CPU_WINDOW_BITMASK	= 7u << 20, /* Chop size (1 << N cycles) */
	DMA_CHCR_ENABLE			= 1u << 24,
	DMA_CHCR_TRIGGER		= 1u << 28,
	DMA_CHCR_PAUSE			= 1u << 29, /* Burst: pauses; Others: blocks TRIGGER autoclear */
	DMA_CHCR_BUS_SNOOP		= 1u << 30  /* Use unknown */
};
typedef enum dma_chcr_flag dma_chcr_flag_t;

/*
 * DMA priority control register macros
 */

#define DMA_DPCR_CH_PRIORITY_BITMASK(N)		(7u            << (4u * (N)))
#define DMA_DPCR_CH_PRIORITY(N, PRIO)		(((PRIO) & 7u) << (4u * (N)))
#define DMA_DPCR_CH_ENABLE(N)			((1u << 3)     << (4u * (N)))

#define DMA_DPCR_CPU_PRIORITY_BITMASK		(7u            << 28)
#define DMA_DPCR_CPU_PRIORITY(PRIO)		(((PRIO) & 7u) << 28)
#define DMA_DPCR_CPU_ENABLE			(1u            << 31) /* Use unknown */

/*
 * DMA interrupt register flag definitions
 */

enum dma_dicr_flag {
	DMA_DICR_CH_MODE_BITMASK	= 0x7Fu <<  0, /* 1 = IRQ@slice; 0 = IRQ@done */
	DMA_DICR_BUS_ERROR		=    1u << 15,
	DMA_DICR_CH_ENABLE_BITMASK	= 0x7Fu << 16,
	DMA_DICR_IRQ_ENABLE		=    1u << 23,
	DMA_DICR_CH_STAT_BITMASK	= 0x7Fu << 24,
	DMA_DICR_IRQ			=    1u << 31,
};
typedef enum dma_dicr_flag dma_dicr_flag_t;

#define DMA_DICR_CH_MODE(N)	(1u << ((N) +  0))
#define DMA_DICR_CH_ENABLE(N)	(1u << ((N) + 16))
#define DMA_DICR_CH_STAT(N)	(1u << ((N) + 24))

/*
 * DMA register addresses
 */

#define DMA_MADR(N)	MMIO32((IO_BASE | 0x080) + (16u * (N)))
#define DMA_BCR(N)	MMIO32((IO_BASE | 0x084) + (16u * (N)))
#define DMA_CHCR(N)	MMIO32((IO_BASE | 0x088) + (16u * (N)))

#define DMA_DPCR	MMIO32(IO_BASE | 0x0F0)
#define DMA_DICR	MMIO32(IO_BASE | 0x0F4)

#endif /* _SYS__DMA_H */
