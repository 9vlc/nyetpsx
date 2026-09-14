/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__TIMER_H
#define _SYS__TIMER_H

#include <sys/mmio.h>

#define TIMER_CTRL_ENABLE_SYNC		(1u <<  0)
#define TIMER_CTRL_SYNC_BITMASK		(3u <<  1)
#define TIMER_CTRL_SYNC_PAUSE		(0u <<  1)
#define TIMER_CTRL_SYNC_RESET1		(1u <<  1)
#define TIMER_CTRL_SYNC_RESET2		(2u <<  1)
#define TIMER_CTRL_SYNC_PAUSE_ONCE	(3u <<  1)
#define TIMER_CTRL_RELOAD		(1u <<  3)
#define TIMER_CTRL_IRQ_ON_RELOAD	(1u <<  4)
#define TIMER_CTRL_IRQ_ON_OVERFLOW	(1u <<  5)
#define TIMER_CTRL_IRQ_REPEAT		(1u <<  6)
#define TIMER_CTRL_IRQ_LATCH		(1u <<  7)
#define TIMER_CTRL_EXT_CLOCK		(1u <<  8)
#define TIMER_CTRL_PRESCALE		(1u <<  9)
#define TIMER_CTRL_IRQ			(1u << 10)
#define TIMER_CTRL_RELOADED		(1u << 11)
#define TIMER_CTRL_OVERFLOWED		(1u << 12)

#define TIMER_VALUE(N)			MMIO16((IO_BASE | 0x100) + (16u * (N)))
#define TIMER_CTRL(N)			MMIO16((IO_BASE | 0x104) + (16u * (N)))
#define TIMER_RELOAD(N)			MMIO16((IO_BASE | 0x108) + (16u * (N)))

#endif /* _SYS__TIMER_H */
