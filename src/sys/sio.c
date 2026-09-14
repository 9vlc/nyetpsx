/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <sys/freq.h>
#include <sys/sio.h>

void
sio_init(int baud)
{
	SIO_CTRL(1) = SIO_CTRL_RESET;
	SIO_MODE(1) = SIO_MODE_BAUD_DIV1 | SIO_MODE_DATA_8 | SIO_MODE_STOP_1;
	SIO_BAUD(1) = F_CPU / baud;
	SIO_CTRL(1) = SIO_CTRL_TX_ENABLE | SIO_CTRL_RX_ENABLE | SIO_CTRL_RTS;
}
