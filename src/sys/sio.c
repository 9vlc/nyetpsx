/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <sys/freq.h>
#include <sys/sio.h>

void
sio_init(int sio, int baud)
{
	SIO_CTRL(sio) = SIO_CTRL_RESET;
	SIO_MODE(sio) = SIO_MODE_BAUD_DIV1 | SIO_MODE_DATA_8 | SIO_MODE_STOP_1;
	SIO_BAUD(sio) = F_CPU / baud;
	SIO_CTRL(sio) = SIO_CTRL_TX_ENABLE | SIO_CTRL_RX_ENABLE | SIO_CTRL_RTS;
}
