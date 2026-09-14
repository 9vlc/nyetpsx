/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

/*
 * "(not) standard (serial) io"
 */

#include <sys/freq.h>
#include <sys/sio.h>
#include <sio.h>

void
sio_init(int baud)
{
	SIO_CTRL(1) = SIO_CTRL_RESET;
	SIO_MODE(1) = SIO_MODE_BAUD_DIV1 | SIO_MODE_DATA_8 | SIO_MODE_STOP_1;
	SIO_BAUD(1) = F_CPU / baud;
	SIO_CTRL(1) = SIO_CTRL_TX_ENABLE | SIO_CTRL_RX_ENABLE | SIO_CTRL_RTS;
}

void
putc(char c)
{
	while ((SIO_STAT(1) & (SIO_STAT_TX_NOT_FULL |
		SIO_STAT_CTS)) == SIO_STAT_CTS) {

		(void)0;
	}

	if (SIO_STAT(1) & SIO_STAT_CTS) {
		SIO_DATA(1) = c;
	}
}

char
getc(void)
{
	while (!(SIO_STAT(1) & SIO_STAT_RX_NOT_EMPTY)) {
		(void)0;
	}

	return (SIO_DATA(1));
}

void
putl(void)
{
	putc('\r');
	putc('\n');
}

int
puts(const char *str)
{
	int len = 1;

	while (*str != '\0') {
		putc(*str);

		str++;
		len++;
	}

	return (len);
}

/*
 * Print a nibble as hex
 */
static inline void
putxn(uint8_t n)
{
	putc("0123456789ABCDEF"[n & 0xF]);
}

/*
 * Print an unsigned integer as hex
 * 8 to 64 bit
 */

void
putx8(uint8_t x)
{
	for (uint32_t i = 0; i < 8; i += 4) {
		putxn(x >> (8 - 4 - i));
	}
}

void
putx16(uint16_t x)
{
	for (uint32_t i = 0; i < 16; i += 4) {
		putxn(x >> (16 - 4 - i));
	}
}


void
putx32(uint32_t x)
{
	for (uint32_t i = 0; i < 32; i += 4) {
		putxn(x >> (32 - 4 - i));
	}
}

void
putx64(uint64_t x)
{
	for (uint64_t i = 0; i < 64; i += 4) {
		putxn(x >> (64 - 4 - i));
	}
}

/*
 * Print a pointer as hex
 */
void
putp(const void *p)
{
	putx32((uint32_t)p);
}

/*
 * Print an unsigned 32-bit integer in base-10
 */
void
putu32(uint32_t v)
{
	char c[10];
	int i = 0;

	if (v == 0) {
		putc('0');
		return;
	}

	while (v > 0) {
		c[i++] = (char)('0' + (v % 10));
		v /= 10;
	}

	while (i > 0) {
		putc(c[--i]);
	}
}

/*
 * Same, just signed
 */
void
puti32(int32_t v)
{
	uint32_t uv;

	if (v < 0) {
		putc('-');
		uv = 0 - (uint32_t)v;
	} else {
		uv = (uint32_t)v;
	}

	putu32(uv);
}
