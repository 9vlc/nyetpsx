/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <sys/ctp.h>
#include <sys/delay.h>
#include <sys/freq.h>
#include <sys/irq.h>
#include <sys/sio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BAUD_RATE	250000
#define DTR_DELAY	30
#define DSR_TIMEOUT	100

void
sio0_init(void)
{
	SIO_CTRL(0) = SIO_CTRL_RESET;

	SIO_MODE(0) = 0
		| SIO_MODE_BAUD_DIV1
		| SIO_MODE_DATA_8;

	SIO_BAUD(0) = F_CPU / BAUD_RATE;

	SIO_CTRL(0) = 0
		| SIO_CTRL_TX_ENABLE
		| SIO_CTRL_RX_ENABLE
		| SIO_CTRL_DSR_IRQ_ENABLE;

	IRQ_MASK &= (uint16_t)(~(1u << IRQ_SIO0));
}

void
sio0_sel_port(int port)
{
	uint16_t cr = ((port) ? SIO_CTRL_CS_PORT_2 : SIO_CTRL_CS_PORT_1);

	SIO_CTRL(0) = (SIO_CTRL(0) & ~SIO_CTRL_CS_PORT_BITMASK) | cr;
}

bool
sio0_await_ack(void)
{
	for (int t_o = DSR_TIMEOUT; t_o > 0; t_o -= 5) {
		if (IRQ_STAT & (1u << IRQ_SIO0)) {
			while (SIO_STAT(0) & SIO_STAT_DSR) {
				__asm__ __volatile__("");
			}

			IRQ_STAT = (uint16_t)(~(1u << IRQ_SIO0));
			SIO_CTRL(0) |= SIO_CTRL_ACKNOWLEDGE;

			return (true);
		}
		delay_us(5);
	}

	return (false);
}

uint8_t
sio0_xchg_byte(uint8_t value)
{
	while (!(SIO_STAT(0) & SIO_STAT_TX_NOT_FULL)) {
		__asm__ __volatile__("");
	}

	SIO_DATA(0) = value;

	while (!(SIO_STAT(0) & SIO_STAT_RX_NOT_EMPTY)) {
		__asm__ __volatile__("");
	}

	return (SIO_DATA(0));
}

size_t
sio0_xchg_ctp_packet(
	ctp_dev_addr_t addr,
	const uint8_t *req,
	uint8_t *resp,
	size_t req_len,
	size_t max_resp_len
	)
{
	while (SIO_STAT(0) & SIO_STAT_RX_NOT_EMPTY) {
		(void)SIO_DATA(0);
	}

	IRQ_STAT = (uint16_t)(~(1u << IRQ_SIO0));
	SIO_CTRL(0) |= SIO_CTRL_DTR | SIO_CTRL_ACKNOWLEDGE;
	delay_us(DTR_DELAY);

	sio0_xchg_byte(addr);

	size_t resp_len = 0;

	for (; resp_len < max_resp_len; resp_len++) {
		if (!sio0_await_ack()) {
			break;
		}

		if (req_len > 0) {
			*(resp++) = sio0_xchg_byte(*(req++));
			req_len--;
		} else {
			*(resp++) = sio0_xchg_byte(0);
		}
	}

	SIO_CTRL(0) &= ~SIO_CTRL_DTR;

	return (resp_len);
}

uint16_t
sio0_poll_pad(int port)
{
	uint8_t req[4], resp[8];

	req[0] = CTP_PAD_READ_DATA;
	req[1] = CTP_TAP_NONE;
	req[2] = 0;
	req[3] = 0;

	sio0_sel_port(port);
	size_t resp_len = sio0_xchg_ctp_packet(
		CTP_ADDR_CONTROLLER,
		req,
		resp,
		sizeof(req),
		sizeof(resp)
	);

	if (resp_len < 4) {
		return (0);
	}

	uint16_t buttons = (resp[2] | (resp[3] << 8)) ^ 0xffff;

	switch (resp[0] >> 4) {
	case CTP_TYPE_DIGITAL:
	case CTP_TYPE_ANALOG_STICK:
	case CTP_TYPE_ANALOG:
	case CTP_TYPE_JOGCON:
		return (buttons);

	case CTP_TYPE_NEGCON:
		/* Remap neGcon analog buttons */
		if (resp_len >= 8) {
			if (resp[5] >= 128) {
				buttons |= BTN_PAD_CROSS;
			}
			if (resp[6] >= 128) {
				buttons |= BTN_PAD_SQUARE;
			}
			if (resp[7] >= 128) {
				buttons |= BTN_PAD_L1;
			}
		}

		return (buttons);

	/* ignore unknown controllers */
	default:
		return (0);
	}
}

void
sio1_init(int baud)
{
	SIO_CTRL(1) = SIO_CTRL_RESET;

	SIO_MODE(1) = 0
		| SIO_MODE_BAUD_DIV1
		| SIO_MODE_DATA_8
		| SIO_MODE_STOP_1;

	SIO_BAUD(1) = F_CPU / baud;

	SIO_CTRL(1) = 0
		| SIO_CTRL_TX_ENABLE
		| SIO_CTRL_RX_ENABLE
		| SIO_CTRL_RTS;
}

char
sio1_getc(void)
{
	while (!(SIO_STAT(1) & SIO_STAT_RX_NOT_EMPTY)) {
		__asm__ __volatile__("");
	}

	return (SIO_DATA(1));
}

void
sio1_putc(char c)
{
	while (!(SIO_STAT(1) & SIO_STAT_TX_NOT_FULL)) {
		__asm__ __volatile__("");
	}

	SIO_DATA(1) = c;
}
