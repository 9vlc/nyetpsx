/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/dma.h>
#include <psx/gpu.h>

#include <graph/geom.h>
#include <graph/gpu.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

void
graph_init_gpu(
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

	gpu_await_cmd();
	GPU_GP0 = gp0_set_page(0, true, false);
	GPU_GP0 = gp0_draw_area_tl(0, 0);
	GPU_GP0 = gp0_draw_area_br(width - 1, height - 1);
	GPU_GP0 = gp0_draw_origin(0, 0);

	gpu_await_dma();
	DMA_DPCR |= DMA_DPCR_CH_ENABLE(DMA_GPU) | DMA_DPCR_CH_ENABLE(DMA_OTC);
	DMA_CHCR(DMA_GPU) = 0;
	DMA_CHCR(DMA_OTC) = 0;
}

void
graph_read_vram(
	void *data,
	graph_rect_t *box
	)
{
	assert(((uint32_t)data % 4) == 0);

	uint32_t len = (box->w * box->h + 1) / 2;
	uint32_t chunk_size, chunk_num;

	if (len < GRAPH_DMA_MAX_CHUNK_SIZE) {
		chunk_size = len;
		chunk_num = 1;
	} else {
		chunk_size = GRAPH_DMA_MAX_CHUNK_SIZE;
		chunk_num = len / GRAPH_DMA_MAX_CHUNK_SIZE;

		assert((len % GRAPH_DMA_MAX_CHUNK_SIZE) == 0);
	}

	gpu_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_NONE);

	gpu_await_cmd();
	GPU_GP0 = gp0_vram_read();
	GPU_GP0 = gp0_xy(box->x, box->y);
	GPU_GP0 = gp0_xy(box->w, box->h);

	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_READ);

	DMA_MADR(DMA_GPU) = (uint32_t)data;
	DMA_BCR(DMA_GPU) = chunk_size | chunk_num << 16;
	DMA_CHCR(DMA_GPU) = 0
		| DMA_CHCR_READ
		| DMA_CHCR_MODE_SLICE
		| DMA_CHCR_ENABLE;
}

void
graph_write_vram(
	const void *data,
	graph_rect_t *box
	)
{
	assert(((uint32_t)data % 4) == 0);

	uint32_t len = (box->w * box->h + 1) / 2;
	uint32_t chunk_size, chunk_num;

	if (len < GRAPH_DMA_MAX_CHUNK_SIZE) {
		chunk_size = len;
		chunk_num = 1;
	} else {
		chunk_size = GRAPH_DMA_MAX_CHUNK_SIZE;
		chunk_num = len / GRAPH_DMA_MAX_CHUNK_SIZE;

		assert((len % GRAPH_DMA_MAX_CHUNK_SIZE) == 0);
	}

	gpu_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_NONE);

	gpu_await_cmd();
	GPU_GP0 = gp0_vram_write();
	GPU_GP0 = gp0_xy(box->x, box->y);
	GPU_GP0 = gp0_xy(box->w, box->h);

	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_WRITE);

	DMA_MADR(DMA_GPU) = (uint32_t)data;
	DMA_BCR(DMA_GPU) = chunk_size | chunk_num << 16;
	DMA_CHCR(DMA_GPU) = 0
		| DMA_CHCR_WRITE
		| DMA_CHCR_MODE_SLICE
		| DMA_CHCR_ENABLE;
}

void
graph_load_texture(
	graph_texture_info_t *info,
	graph_texture_data_t *data
	)
{
	assert((data->w <= 256) && (data->h <= 256));

	int ncol = 0, wdiv = 0;
	gp0_color_depth_t depth = data->depth;

	if (data->clut != NULL) {
		assert(depth != GP0_COLOR_16BPP);

		ncol = (depth == GP0_COLOR_8BPP ? 256 : 16);
		wdiv = (depth == GP0_COLOR_8BPP ? 1 : 2);

		assert(!(data->cx % 16) && ((data->cx + ncol) <= 1024));

		graph_write_vram(data->clut, &GRAPH_RECT(
			data->cx,
			data->cy,
			ncol,
			1
		));
		gpu_await_dma();
	} else {
		depth = GP0_COLOR_16BPP;
	}

	graph_write_vram(data->img, &GRAPH_RECT(
		data->x,
		data->y,
		data->w >> wdiv,
		data->h
	));
	gpu_await_dma();

	GPU_GP0 = gp0_flush_cache();

	info->page = gp0_page(
		data->x / 64,
		data->y / 256,
		GP0_BLEND_SEMITRANS,
		depth
	);
	info->clut = (data->clut != NULL ? gp0_clut(data->cx / 16, data->cy) : 0);
	info->u    = (uint8_t)((data->x % 64) << wdiv);
	info->v    = (uint8_t)(data->y % 256);
	info->w    = (uint16_t)data->w;
	info->h    = (uint16_t)data->h;
}

uint32_t *
graph_alloc_chain_packet(
	graph_dma_chain_t *chain,
	size_t cmds
	)
{
	assert(cmds <= GRAPH_DMA_MAX_CHUNK_SIZE);

	uint32_t *ptr = chain->next;
	chain->next += cmds + 1;

	ptr[0] = gp0_tag(cmds, chain->next);

	assert(chain->next < &(chain->data)[GRAPH_DMA_CHAIN_BUF_SIZE]);

	return (&ptr[1]);
}

uint32_t *
graph_alloc_ord_chain_packet(
	graph_dma_ord_chain_t *chain,
	uint32_t z,
	size_t cmds
	)
{
	assert(cmds <= GRAPH_DMA_MAX_CHUNK_SIZE);
	assert(z < GRAPH_DMA_CHAIN_OT_SIZE);

	uint32_t *ptr = chain->next;
	chain->next += cmds + 1;

	ptr[0] = gp0_tag(cmds, (void *)chain->ot[z]);
	chain->ot[z] = gp0_tag(0, ptr);

	assert(chain->next < &(chain->data)[GRAPH_DMA_CHAIN_BUF_SIZE]);

	return (&ptr[1]);
}

void
graph_send_chain(
	const uint32_t *data
	)
{
	gpu_await_dma();
	GPU_GP1 = gp1_dma_request_mode(GP1_DREQ_GP0_WRITE);

	DMA_MADR(DMA_GPU) = (uint32_t)data;
	DMA_CHCR(DMA_GPU) = 0
		| DMA_CHCR_WRITE
		| DMA_CHCR_MODE_LIST
		| DMA_CHCR_ENABLE;
}

void
graph_clear_ot(
	uint32_t *ot,
	size_t size
	)
{
	assert(!((uint32_t)ot % 4));

	DMA_MADR(DMA_OTC) = (uint32_t)&ot[size - 1];
	DMA_BCR(DMA_OTC) = size;
	DMA_CHCR(DMA_OTC) = 0
		| DMA_CHCR_READ
		| DMA_CHCR_REVERSE
		| DMA_CHCR_MODE_BURST
		| DMA_CHCR_ENABLE
		| DMA_CHCR_TRIGGER;

	while (DMA_CHCR(DMA_OTC) & DMA_CHCR_ENABLE) {
		__asm__ __volatile__("");
	}
}
