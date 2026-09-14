/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__IRQ_H
#define _SYS__IRQ_H

#include <sys/mmio.h>

#define IRQ_VSYNC	0
#define IRQ_GPU		1
#define IRQ_CDROM	2
#define IRQ_DMA		3
#define IRQ_TIMER0	4
#define IRQ_TIMER1	5
#define IRQ_TIMER2	6
#define IRQ_SIO0	7
#define IRQ_SIO1	8
#define IRQ_SPU		9
#define IRQ_GUN		10
#define IRQ_PIO		10

#define IRQ_STAT	MMIO16(IO_BASE | 0x070)
#define IRQ_MASK	MMIO16(IO_BASE | 0x074)

#endif /* _SYS__IRQ_H */
