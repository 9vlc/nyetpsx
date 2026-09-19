/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__IRQ_H
#define _SYS__IRQ_H

#include <sys/mmio.h>

/*
 * Interrupt channel definitions
 *
 * I_STAT and I_MASK share the same bit assignments. I_STAT is edge triggered
 * and must be acknowledged by writing the corresponding bit back to it.
 */

enum irq_channel {
	IRQ_VSYNC	=  0, /* Vblank */
	IRQ_GPU		=  1,
	IRQ_CDROM	=  2,
	IRQ_DMA		=  3,
	IRQ_TIMER0	=  4,
	IRQ_TIMER1	=  5,
	IRQ_TIMER2	=  6,
	IRQ_SIO0	=  7, /* Controller and memory card byte received */
	IRQ_SIO1	=  8, /* Second serial port */
	IRQ_SPU		=  9,
	IRQ_GUN		= 10, /* Shares bit with IRQ_PIO */
	IRQ_PIO		= 10, /* Shares bit with IRQ_GUN */
};
typedef enum irq_channel irq_channel_t;

#define IRQ_STAT	MMIO16(IO_BASE | 0x070)
#define IRQ_MASK	MMIO16(IO_BASE | 0x074)

#endif /* _SYS__IRQ_H */
