/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/bus.h>
#include <psx/cdrom.h>
#include <psx/delay.h>
#include <psx/dma.h>
#include <psx/spu.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define IRQ_TIMEOUT 5000000

void
cdrom_init(
	void
	)
{
	BIU_DEV5_CTRL = 0
		| BIU_CTRL_WRITE_DELAY(3)
		| BIU_CTRL_READ_DELAY(4)
		| BIU_CTRL_RECOVERY
		| BIU_CTRL_PRESTROBE
		| BIU_CTRL_WIDTH_8
		| BIU_CTRL_SIZE(2);

	BIU_COM_DELAY = 0
		| BIU_COM_RECOVERY(5)
		| BIU_COM_HOLD(2)
		| BIU_COM_FLOAT(3)
		| BIU_COM_PRESTROBE(1);

	CDROM_ADDRESS = 0;
	CDROM_HCHPCTL = 0;
	CDROM_ADDRESS = 1;
	CDROM_HINTMSK_W = 0;
	CDROM_HCLRCTL = 0
		| CDROM_HCLRCTL_CLRINT_BITMASK
		| CDROM_HCLRCTL_CLRBFEMPT
		| CDROM_HCLRCTL_CLRBFWRDY;

	DMA_DPCR |= DMA_DPCR_CH_ENABLE(DMA_CDROM);
	DMA_CHCR(DMA_CDROM) = 0;

	cdrom_cmd(CDROM_CMD_INIT, 0, 0);

	cdrom_irq_type_t irq;

	irq = cdrom_await_irq(0, 0);
	assert(irq == CDROM_IRQ_ACKNOWLEDGE);

	irq = cdrom_await_irq(0, 0);
	assert(irq == CDROM_IRQ_COMPLETE);
}

void
cdrom_setup_audio(
	bool mono
	)
{
	CDROM_ADDRESS = 2;
	CDROM_ATV0    = mono ? 64 : 128; /* L -> L */
	CDROM_ATV1    = mono ? 64 :   0; /* L -> R */
	CDROM_ADDRESS = 3;
	CDROM_ATV2    = mono ? 64 : 128; /* R -> R */
	CDROM_ATV3    = mono ? 64 :   0; /* R -> L */
	CDROM_ADPCTL  = CDROM_ADPCTL_CHNGATV;

	SPU_CTRL |= SPU_CTRL_I2SA_ENABLE;
	SPU_AVOLL = 0x7FFF;
	SPU_AVOLR = 0x7FFF;
}

void
cdrom_await_dma(
	void
	)
{
	while (DMA_CHCR(DMA_CDROM) & DMA_CHCR_ENABLE) {
		__asm__ __volatile__("");
	}
}

void
cdrom_cmd(
	cdrom_cmd_t cmd,
	const uint8_t *params,
	size_t param_len
	)
{
	assert(param_len <= 16);

	CDROM_ADDRESS = 1;
	CDROM_HCLRCTL = CDROM_HCLRCTL_CLRPRM;
	delay_us(1);

	CDROM_ADDRESS = 0;

	for (; param_len > 0; param_len--) {
		CDROM_PARAMETER = *(params++);
	}

	CDROM_COMMAND = cmd;
}

cdrom_irq_type_t
cdrom_check_irq(
	uint8_t *resp,
	size_t max_resp_len
	)
{
	CDROM_ADDRESS = 1;

	int irq1, irq2;

	do {
		irq1 = CDROM_HINTSTS & CDROM_HINT_INT_BITMASK;
		irq2 = CDROM_HINTSTS & CDROM_HINT_INT_BITMASK;
	} while (irq1 != irq2);

	if (irq1) {
		for (; max_resp_len > 0; max_resp_len--) {
			if (!(CDROM_HSTS & CDROM_HSTS_RSLRRDY)) {
				break;
			}

			*(resp++) = CDROM_RESULT;
		}

		CDROM_HCLRCTL = CDROM_HCLRCTL_CLRINT_BITMASK;
	}

	return (irq1);
}

cdrom_irq_type_t
cdrom_await_irq(
	uint8_t *resp,
	size_t max_resp_len
	)
{
	for (int timeout = IRQ_TIMEOUT; timeout > 0; timeout -= 5) {
		cdrom_irq_type_t irq = cdrom_check_irq(resp, max_resp_len);

		if (irq) {
			return (irq);
		}

		delay_us(5);
	}

	return (CDROM_IRQ_NONE);
}

bool
cdrom_play_cdda(
	uint32_t track,
	uint8_t mode
	)
{
	cdrom_check_irq(0, 0);

	mode |= CDROM_MODE_CDDA;

	cdrom_cmd(CDROM_CMD_SETMODE, &mode, sizeof(mode));
	if (cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) {
		return (false);
	}

	uint8_t bcd_track = cdrom_encode_bcd(track);

	if (track > 0) {
		cdrom_cmd(CDROM_CMD_PLAY, &bcd_track,
		    sizeof(bcd_track));
	} else {
		cdrom_cmd(CDROM_CMD_PLAY, 0, 0);
	}
	if (cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) {
		return (false);
	}

	return (true);
}

bool
cdrom_start_read(
	uint32_t lba,
	uint8_t mode,
	bool retry
	)
{
	cdrom_check_irq(0, 0);

	uint8_t msf[3];

	cdrom_lba_to_msf((cdrom_msf_t *) msf, lba);

	cdrom_cmd(CDROM_CMD_SETLOC, msf, sizeof(msf));
	if (cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) {
		return (false);
	}

	cdrom_cmd(CDROM_CMD_SETMODE, &mode, sizeof(mode));
	if (cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) {
		return (false);
	}

	cdrom_cmd(retry ? CDROM_CMD_READ_N : CDROM_CMD_READ_S, 0, 0);
	if (cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) {
		return (false);
	}

	return (true);
}

bool
cdrom_pause_read(
	void
	)
{
	cdrom_check_irq(0, 0);

	cdrom_cmd(CDROM_CMD_PAUSE, 0, 0);
	if ((cdrom_await_irq(0, 0) != CDROM_IRQ_ACKNOWLEDGE) ||
		(cdrom_await_irq(0, 0) != CDROM_IRQ_COMPLETE)) {

		return (false);
	}

	return (true);
}

void
cdrom_read_sector(
	void *data,
	size_t len
	)
{
	cdrom_await_dma();
	assert(!((uintptr_t)data % 4));

	len = (len + 3) / 4;

	CDROM_ADDRESS = 0;
	CDROM_HCHPCTL = CDROM_HCHPCTL_BFRD;

	while (!(CDROM_HSTS & CDROM_HSTS_DRQSTS)) {
		__asm__ __volatile__("");
	}

	DMA_MADR(DMA_CDROM) = (uintptr_t)data;
	DMA_BCR (DMA_CDROM) = len;
	DMA_CHCR(DMA_CDROM) = 0
		| DMA_CHCR_READ
		| DMA_CHCR_MODE_BURST
		| DMA_CHCR_ENABLE
		| DMA_CHCR_TRIGGER;
}

size_t
cdrom_read_sectors(
	void *data,
	uint32_t lba,
	size_t num_sectors,
	uint8_t mode
	)
{
	if (!cdrom_start_read(lba, mode, true)) {
		return (0);
	}

	uint8_t *ptr = (uint8_t *)data;
	size_t sector_len = cdrom_sector_len(mode);
	size_t sectors_read = 0;

	for (; sectors_read < num_sectors; sectors_read++) {
		if (cdrom_await_irq(0, 0) != CDROM_IRQ_DATA_READY) {
			break;
		}

		cdrom_read_sector(ptr, sector_len);
		cdrom_await_dma();

		ptr += sector_len;
	}

	cdrom_pause_read();

	return (sectors_read);
}
