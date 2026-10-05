/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <sys/dma.h>
#include <sys/irq.h>
#include <sys/gpu.h>

#include <graph/geom.h>
#include <graph/gpu.h>
#include <graph/directdraw.h>
#include <graph/dmadraw.h>

#include <assert.h>
#include <stdint.h>

void
graph_await_dma(void)
{
	while (DMA_CHCR(DMA_GPU) & DMA_CHCR_ENABLE) {
		__asm__ volatile("");
	}

	while (!(GPU_GP1 & GP1_STAT_WRITE_READY)) {
		__asm__ __volatile__("");
	}
}

void
graph_await_cmd(void)
{
	while (!(GPU_GP1 & GP1_STAT_CMD_READY)) {
		__asm__ __volatile__("");
	}
}

void
graph_await_vsync(void)
{
	while (!(IRQ_STAT & (1 << IRQ_VSYNC))) {
		__asm__ __volatile__("");
	}

	IRQ_STAT = ~(1 << IRQ_VSYNC);
}

void
graph_gpu_init(
	gp1_hres_t hres,
	gp1_vres_t vres,
	int width,
	int height
	)
{
	bool is_pal = ((GPU_GP1 & GP1_STAT_FB_MODE_BITMASK) == GP1_STAT_FB_MODE_PAL);

	int x = 0x760;
	int y = (is_pal ? 0xA3 : 0x88);

	int off_x = (width * gp1_clock_mult_h(hres)) / 2;
	int off_y = (height / gp1_clock_div_v(vres)) / 2;

	GPU_GP1 = gp1_reset_gpu();
	GPU_GP1 = gp1_fb_range_h(x - off_x, x + off_x);
	GPU_GP1 = gp1_fb_range_v(y - off_y, y + off_y);
	GPU_GP1 = gp1_fb_mode(
		hres,
		vres,
		(is_pal ? GP1_MODE_PAL : GP1_MODE_NTSC),
		GP1_COLOR_16BPP,
		(vres == GP1_VRES_512) /* Interlace on hires */
	);
	GPU_GP1 = gp1_disp_blank(false);

	graph_await_dma();
	DMA_DPCR |= DMA_DPCR_CH_ENABLE(DMA_GPU);
	DMA_CHCR(DMA_GPU) = 0;

	graph_await_cmd();
	GPU_GP0 = gp0_set_page(0, true, false);
	GPU_GP0 = gp0_fb_offset1(0, 0);
	GPU_GP0 = gp0_fb_offset2(width, height);
	GPU_GP0 = gp0_fb_origin(0, 0);
}

void
graph_vram_read(
	void *data,
	graph_rect_t *box
	)
{
	/* Data pointer must be aligned */
	assert(((uint32_t)data % 4) == 0);

	uint32_t len = (box->w * box->h + 1) / 2;
	uint32_t chunk_size, chunk_num;

	if (len < GRAPH_DMA_MAX_CHUNK_SIZE) {
		chunk_size = len;
		chunk_num = 1;
	} else {
		chunk_size = GRAPH_DMA_MAX_CHUNK_SIZE;
		chunk_num = len / GRAPH_DMA_MAX_CHUNK_SIZE;

		assert((len % GRAPH_DMA_MAX_CHUNK_SIZE) != 0);
	}

	graph_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_NONE);

	graph_await_cmd();
	GPU_GP0 = gp0_vram_read();
	GPU_GP0 = gp0_xy(box->x, box->y);
	GPU_GP0 = gp0_xy(box->w, box->h);

	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_READ);

	DMA_MADR(DMA_GPU) = (uint32_t)data;
	DMA_BCR(DMA_GPU)  = chunk_size | chunk_num << 16;
	DMA_CHCR(DMA_GPU) = DMA_CHCR_READ | DMA_CHCR_MODE_SLICE | DMA_CHCR_ENABLE;
}

void
graph_vram_write(
	const void *data,
	graph_rect_t *box
	)
{
	/* Data pointer must be aligned */
	assert(((uint32_t)data % 4) == 0);

	uint32_t len = (box->w * box->h + 1) / 2;
	uint32_t chunk_size, chunk_num;

	if (len < GRAPH_DMA_MAX_CHUNK_SIZE) {
		chunk_size = len;
		chunk_num = 1;
	} else {
		chunk_size = GRAPH_DMA_MAX_CHUNK_SIZE;
		chunk_num = len / GRAPH_DMA_MAX_CHUNK_SIZE;

		assert((len % GRAPH_DMA_MAX_CHUNK_SIZE) != 0);
	}

	graph_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_NONE);

	graph_await_cmd();
	GPU_GP0 = gp0_vram_write();
	GPU_GP0 = gp0_xy(box->x, box->y);
	GPU_GP0 = gp0_xy(box->w, box->h);

	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_WRITE);

	DMA_MADR(DMA_GPU) = (uint32_t)data;
	DMA_BCR(DMA_GPU)  = chunk_size | chunk_num << 16;
	DMA_CHCR(DMA_GPU) = DMA_CHCR_WRITE | DMA_CHCR_MODE_SLICE | DMA_CHCR_ENABLE;
}
/*
uint32_t
graph_chain_alloc_packet()*/

void
graph_chain_send(const graph_dma_link_t *dma_chain)
{
	assert(((uint32_t)dma_chain % 4) == 0);

	graph_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_WRITE);

	DMA_MADR(DMA_GPU) = (uint32_t)dma_chain;
	DMA_CHCR(DMA_GPU) = DMA_CHCR_WRITE | DMA_CHCR_MODE_LIST | DMA_CHCR_ENABLE;
}
