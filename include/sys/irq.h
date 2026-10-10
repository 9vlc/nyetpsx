/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__IRQ_H
#define _SYS__IRQ_H

#include <sys/mmio.h>

#include <stdint.h>

/*
 * Interrupt channel definitions
 */

#define IRQ_VSYNC	0 /* Vblank */
#define IRQ_GPU		1
#define IRQ_CDROM	2
#define IRQ_DMA		3
#define IRQ_TIMER0	4
#define IRQ_TIMER1	5
#define IRQ_TIMER2	6
#define IRQ_SIO0	7 /* Controller and memory card byte received */
#define IRQ_SIO1	8 /* Second serial port */
#define IRQ_SPU		9
#define IRQ_GUN		10 /* Shares bit with IRQ_PIO */
#define IRQ_PIO		10 /* Shares bit with IRQ_GUN */
typedef uint32_t irq_channel_t;

#define IRQ_STAT	MMIO16(IO_BASE | 0x070)
#define IRQ_MASK	MMIO16(IO_BASE | 0x074)

#endif /* _SYS__IRQ_H */
