/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/bus.h>
#include <sys/delay.h>
#include <sys/dma.h>
#include <sys/spu.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define DMA_MAX_CHUNK_SIZE 16

#define DUMMY_BLOCK_OFFSET	(SPU_RAM_SIZE - 32)
#define REVERB_BUFFER_OFFSET	(SPU_RAM_SIZE - 16)

static const uint8_t dummy[] = {
	0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void
spu_await_dma(
	void
	)
{
	while (DMA_CHCR(DMA_SPU) & DMA_CHCR_ENABLE) {
		__asm__ __volatile__("");
	}

	delay_us(2 * DMA_MAX_CHUNK_SIZE);
}

void
spu_init(
	void
	)
{
	BIU_DEV4_CTRL = 0
		| BIU_CTRL_WRITE_DELAY(1)
		| BIU_CTRL_READ_DELAY(14)
		| BIU_CTRL_RECOVERY
		| BIU_CTRL_WIDTH_16
		| BIU_CTRL_AUTO_INCR
		| BIU_CTRL_SIZE(9)
		| BIU_CTRL_DMA_DELAY(0)
		| BIU_CTRL_DMA_DELAY_ENABLE;

	BIU_COM_DELAY = 0
		| BIU_COM_RECOVERY(5)
		| BIU_COM_HOLD(2)
		| BIU_COM_FLOAT(3)
		| BIU_COM_PRESTROBE(1);

	SPU_CTRL = 0
		| SPU_CTRL_XFER_NONE
		| SPU_CTRL_ENABLE;

	SPU_RAM_CTRL = 0
		| SPU_RAM_CTRL_BANKS_1
		| SPU_RAM_CTRL_SIZE_512KB;

	SPU_MVOLL = 0x3fff & ~SPU_VOL_SLIDE_ENABLE;
	SPU_MVOLR = 0x3fff & ~SPU_VOL_SLIDE_ENABLE;
	SPU_EVOLL = 0;
	SPU_EVOLR = 0;
	SPU_ESA = REVERB_BUFFER_OFFSET / SPU_RAM_ADDR_UNIT;

	DMA_DPCR |= DMA_DPCR_CH_ENABLE(DMA_SPU);
	DMA_CHCR(DMA_SPU) = 0;

	spu_send_data(dummy, DUMMY_BLOCK_OFFSET, sizeof(dummy));
	spu_await_dma();
	spu_reset();

	SPU_CTRL = 0
		| SPU_CTRL_XFER_NONE
		| SPU_CTRL_DAC_ENABLE
		| SPU_CTRL_ENABLE;
}

static void
spu_setup_xfer(
	const void *data,
	uint32_t off,
	size_t len,
	uint16_t xfer_mode,
	uint16_t dma_cycles
	)
{
	assert(!((uintptr_t)(data) % 4));
	assert(!(off % SPU_RAM_ADDR_UNIT));
	assert((off + len) <= SPU_RAM_SIZE);

	len = (len + 3) / 4;
	size_t chunk_len, num_chunks;

	if (len < DMA_MAX_CHUNK_SIZE) {
		chunk_len = len;
		num_chunks = 1;
	} else {
		chunk_len = DMA_MAX_CHUNK_SIZE;
		num_chunks = len / DMA_MAX_CHUNK_SIZE;

		assert(!(len % DMA_MAX_CHUNK_SIZE));
	}

	uint16_t attr = SPU_CTRL & ~SPU_CTRL_XFER_BITMASK;
	SPU_CTRL = attr;

	while ((SPU_STAT & SPU_STAT_XFER_BITMASK) != SPU_STAT_XFER_NONE) {
		__asm__ __volatile__("");
	}

	SPU_TSA = off / SPU_RAM_ADDR_UNIT;
	SPU_CTRL = attr | xfer_mode;

	while ((SPU_STAT & SPU_STAT_XFER_BITMASK) != xfer_mode) {
		__asm__ __volatile__("");
	}

	uint32_t ctrl = BIU_DEV4_CTRL & ~BIU_CTRL_DMA_DELAY_BITMASK;
	BIU_DEV4_CTRL = ctrl | BIU_CTRL_DMA_DELAY(dma_cycles);

	DMA_MADR(DMA_SPU) = (uintptr_t)(data);
	DMA_BCR(DMA_SPU) = chunk_len | (num_chunks << 16);
	DMA_CHCR(DMA_SPU) = 0
		| ((xfer_mode == SPU_CTRL_XFER_DMA_WRITE) ? DMA_CHCR_WRITE : DMA_CHCR_READ)
		| DMA_CHCR_MODE_SLICE
		| DMA_CHCR_ENABLE;
}

void
spu_send_data(
	const void *data,
	uint32_t off,
	size_t len
	)
{
	spu_await_dma();
	spu_setup_xfer(data, off, len, SPU_CTRL_XFER_DMA_WRITE, 0);
}

void
spu_recv_data(
	void *data,
	uint32_t off,
	size_t len
	)
{
	spu_await_dma();
	spu_setup_xfer(data, off, len, SPU_CTRL_XFER_DMA_READ, 2);
}

void
spu_reset(
	void
	)
{
	for (int i = 0; i < SPU_NUM_CHANNELS; i++) {
		SPU_CH_VOLL(i) = 0;
		SPU_CH_VOLR(i) = 0;
		SPU_CH_PITCH(i) = SPU_PITCH_UNIT;
		SPU_CH_SSA(i) = DUMMY_BLOCK_OFFSET / SPU_RAM_ADDR_UNIT;
	}

	SPU_PMON0 = 0;
	SPU_PMON1 = 0;
	SPU_NON0 = 0;
	SPU_NON1 = 0;
	SPU_EON0 = 0;
	SPU_EON1 = 0;

	SPU_KON0 = 0xFFFF;
	SPU_KON1 = 0x00FF;
}

int
spu_find_free_channel(
	void
	)
{
	for (int i = 0; i < SPU_NUM_CHANNELS; i++) {
		if (!SPU_CH_ENVX(i)) {
			return (i);
		}
	}

	return (-1);
}

int
spu_play_sample(
	uint32_t off,
	uint32_t rate,
	int16_t vol
	)
{
	int ch = spu_find_free_channel();

	if (ch >= 0) {
		SPU_CH_VOLL(ch) = (vol / 2) & ~SPU_VOL_SLIDE_ENABLE;
		SPU_CH_VOLR(ch) = (vol / 2) & ~SPU_VOL_SLIDE_ENABLE;
		SPU_CH_PITCH(ch) = (rate * SPU_PITCH_UNIT) / 44100;
		SPU_CH_SSA(ch) = off / SPU_RAM_ADDR_UNIT;

		SPU_CH_ADSR1(ch) = 0
			| SPU_ADSR1_SL(15)
			| SPU_ADSR1_DR(15)
			| SPU_ADSR1_AR(0);

		SPU_CH_ADSR2(ch) = 0
			| SPU_ADSR2_RR(0)
			| SPU_ADSR2_SR(0);

		SPU_CH_ENVX (ch) = vol;

		if (ch < 16) {
			SPU_KON0 = 1 << ch;
		} else {
			SPU_KON1 = 1 << (ch - 16);
		}
	}

	return (ch);
}
