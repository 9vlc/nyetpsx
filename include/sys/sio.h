/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__SIO_H
#define _SYS__SIO_H

#include <sys/mmio.h>

/*
 * SIO status flags
 */

enum sio_status_flag {
	SIO_STAT_TX_NOT_FULL		= 1u << 0,
	SIO_STAT_RX_NOT_EMPTY		= 1u << 1,
	SIO_STAT_TX_EMPTY		= 1u << 2,
	SIO_STAT_RX_PARITY_ERR		= 1u << 3,
	SIO_STAT_RX_OVERRUN		= 1u << 4, /* SIO1 only */
	SIO_STAT_RX_STOP_ERR		= 1u << 5, /* SIO1 only */
	SIO_STAT_RX_INVERT		= 1u << 6, /* SIO1 only */
	SIO_STAT_DSR			= 1u << 7, /* DSR is /ACK on SIO0 */
	SIO_STAT_CTS			= 1u << 8, /* SIO1 only */
	SIO_STAT_IRQ			= 1u << 9,
};
typedef enum sio_status_flag sio_status_flag_t;

/*
 * SIO mode flags
 */

enum sio_mode_flag {
	SIO_MODE_BAUD_BITMASK		= 3u << 0,
	SIO_MODE_BAUD_DIV1		= 1u << 0,
	SIO_MODE_BAUD_DIV16		= 2u << 0,
	SIO_MODE_BAUD_DIV64		= 3u << 0,
	SIO_MODE_DATA_BITMASK		= 3u << 2,
	SIO_MODE_DATA_5			= 0u << 2,
	SIO_MODE_DATA_6			= 1u << 2,
	SIO_MODE_DATA_7			= 2u << 2,
	SIO_MODE_DATA_8			= 3u << 2,
	SIO_MODE_PARITY_BITMASK		= 3u << 4,
	SIO_MODE_PARITY_NONE		= 0u << 4,
	SIO_MODE_PARITY_EVEN		= 1u << 4,
	SIO_MODE_PARITY_ODD		= 3u << 4,
	SIO_MODE_STOP_BITMASK		= 3u << 6, /* SIO1 only */
	SIO_MODE_STOP_1			= 1u << 6, /* SIO1 only */
	SIO_MODE_STOP_1_5		= 2u << 6, /* SIO1 only */
	SIO_MODE_STOP_2			= 3u << 6, /* SIO1 only */
	SIO_MODE_SCK_INVERT		= 1u << 8, /* SIO0 only */
};
typedef enum sio_mode_flag sio_mode_flag_t;

/*
 * SIO control flags
 */

enum sio_control_flag {
	SIO_CTRL_TX_ENABLE		= 1u << 0,
	SIO_CTRL_DTR			= 1u << 1, /* DTR is /CS on SIO0 */
	SIO_CTRL_RX_ENABLE		= 1u << 2,
	SIO_CTRL_TX_INVERT		= 1u << 3, /* SIO1 only */
	SIO_CTRL_ACKNOWLEDGE		= 1u << 4,
	SIO_CTRL_RTS			= 1u << 5, /* SIO1 only */
	SIO_CTRL_RESET			= 1u << 6,
	SIO_CTRL_TX_IRQ_ENABLE		= 1u << 10,
	SIO_CTRL_RX_IRQ_ENABLE		= 1u << 11,
	SIO_CTRL_DSR_IRQ_ENABLE		= 1u << 12, /* DSR is /ACK on SIO0 */
	SIO_CTRL_CS_PORT_1		= 0u << 13, /* SIO0 only */
	SIO_CTRL_CS_PORT_2		= 1u << 13, /* SIO0 only */
};
typedef enum sio_control_flag sio_control_flag_t;

/*
 * SIO_DATA is a 32-bit register, but some emulators do not implement it
 *   correctly and break if it's read more than 8 bits at a time.
 */
#define SIO_DATA(N)			MMIO8((IO_BASE | 0x040) + (16u * (N)))
#define SIO_STAT(N)			MMIO16((IO_BASE | 0x044) + (16u * (N)))
#define SIO_MODE(N)			MMIO16((IO_BASE | 0x048) + (16u * (N)))
#define SIO_CTRL(N)			MMIO16((IO_BASE | 0x04A) + (16u * (N)))
#define SIO_BAUD(N)			MMIO16((IO_BASE | 0x04E) + (16u * (N)))

void sio_init(int sio, int baud);

#endif /* _SYS__SIO_H */
