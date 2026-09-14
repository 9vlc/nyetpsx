/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__GPU_H
#define _SYS__GPU_H

#include <sys/mmio.h>

#include <stdbool.h>
#include <stdint.h>

#define DEF(type) static inline type __attribute__((always_inline))

#define GPU_GP0 MMIO32(IO_BASE | 0x810)
#define GPU_GP1 MMIO32(IO_BASE | 0x814)

#define GP1_STAT_PAGE_X_BITMASK		(15u << 0) /* GP0_CMD_TPAGE */
#define GP1_STAT_PAGE_Y0		(1u << 4) /* GP0_CMD_TPAGE */
#define GP1_STAT_BLEND_BITMASK		(3u << 5) /* GP0_CMD_TPAGE */
#define GP1_STAT_BLEND_SEMITRANS	(0u << 5) /* GP0_CMD_TPAGE */
#define GP1_STAT_BLEND_ADD		(1u << 5) /* GP0_CMD_TPAGE */
#define GP1_STAT_BLEND_SUBTRACT		(2u << 5) /* GP0_CMD_TPAGE */
#define GP1_STAT_BLEND_DIV4_ADD		(3u << 5) /* GP0_CMD_TPAGE */
#define GP1_STAT_COLOR_BITMASK		(3u << 7) /* GP0_CMD_TPAGE */
#define GP1_STAT_COLOR_4BPP		(0u << 7) /* GP0_CMD_TPAGE */
#define GP1_STAT_COLOR_8BPP		(1u << 7) /* GP0_CMD_TPAGE */
#define GP1_STAT_COLOR_16BPP		(2u << 7) /* GP0_CMD_TPAGE */
#define GP1_STAT_DITHER			(1u << 9) /* GP0_CMD_TPAGE */
#define GP1_STAT_UNLOCK_FB		(1u << 10) /* GP0_CMD_TPAGE */
#define GP1_STAT_SET_MASK		(1u << 11) /* GP0_CMD_FB_MASK */
#define GP1_STAT_USE_MASK		(1u << 12) /* GP0_CMD_FB_MASK */
#define GP1_STAT_DISP_FIELD_BITMASK	(1u << 13)
#define GP1_STAT_DISP_FIELD_EVEN	(0u << 13)
#define GP1_STAT_DISP_FIELD_ODD		(1u << 13)
#define GP1_STAT_PAGE_Y1		(1u << 15) /* GP0_CMD_TPAGE */
#define GP1_STAT_FB_HRES_BITMASK	(7u << 16) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_VRES_BITMASK	(1u << 19) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_VRES_256		(0u << 19) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_VRES_512		(1u << 19) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_MODE_BITMASK	(1u << 20) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_MODE_NTSC		(0u << 20) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_MODE_PAL		(1u << 20) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_COLOR_BITMASK	(1u << 21) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_COLOR_16BPP		(0u << 21) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_COLOR_24BPP		(1u << 21) /* GP1_CMD_FB_MODE */
#define GP1_STAT_FB_INTERLACE		(1u << 22) /* GP1_CMD_FB_MODE */
#define GP1_STAT_DISP_BLANK		(1u << 23) /* GP1_CMD_DISP_BLANK */
#define GP1_STAT_IRQ			(1u << 24)
#define GP1_STAT_DREQ			(1u << 25)
#define GP1_STAT_CMD_READY		(1u << 26)
#define GP1_STAT_READ_READY		(1u << 27)
#define GP1_STAT_WRITE_READY		(1u << 28)
#define GP1_STAT_DREQ_MODE_BITMASK	(3u << 29) /* GP1_CMD_DREQ_MODE */
#define GP1_STAT_DREQ_MODE_NONE		(0u << 29) /* GP1_CMD_DREQ_MODE */
#define GP1_STAT_DREQ_MODE_FIFO		(1u << 29) /* GP1_CMD_DREQ_MODE */
#define GP1_STAT_DREQ_MODE_GP0_WRITE	(2u << 29) /* GP1_CMD_DREQ_MODE */
#define GP1_STAT_DREQ_MODE_GP0_READ	(3u << 29) /* GP1_CMD_DREQ_MODE */
#define GP1_STAT_DRAW_FIELD_BITMASK	(1u << 31)
#define GP1_STAT_DRAW_FIELD_EVEN	(0u << 31)
#define GP1_STAT_DRAW_FIELD_ODD		(1u << 31)

/*
 * Drawing attributes
 */

#define GP0_BLEND_BITMASK	3
#define GP0_BLEND_SEMITRANS	0
#define GP0_BLEND_ADD		1
#define GP0_BLEND_SUBTRACT	2
#define GP0_BLEND_DIV4_ADD	3

#define GP0_COLOR_BITMASK	3
#define GP0_COLOR_4BPP		0
#define GP0_COLOR_8BPP		1
#define GP0_COLOR_16BPP		2

DEF(uint16_t)
gp0_page(uint32_t x, uint32_t y, uint32_t blend_mode, uint32_t depth)
{
	return (((x & 15) << 0) |
		((y & 1) << 4) |
		((blend_mode & 3) <<  5) |
		((depth & 3) <<  7) |
		((y & 2) << 10));
}
DEF(uint16_t)
gp0_clut(uint32_t x, uint32_t y)
{
	return (((x & 0x03F) << 0) |
		((y & 0x3FF) << 6));
}

DEF(uint32_t)
gp0_xy(int32_t x, int32_t y)
{
	return (((x & 0xFFFF) << 0) |
		((y & 0xFFFF) << 16));
}

DEF(uint32_t)
gp0_uv(uint32_t u, uint32_t v, uint16_t attr)
{
	return (((u & 0x00FF) << 0) |
		((v & 0x00FF) << 8) |
		((attr & 0xFFFF) << 16));
}

DEF(uint32_t)
gp0_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r << 0) |
		(g << 8) |
		(b << 16));
}

/*
 * DMA tags
 */

DEF(uint32_t)
gp0_tag(size_t length, void *next)
{
	return ((((uint32_t)next & 0xFFFFFF) <<  0) |
		(((uint32_t)length & 0x0000FF) << 24));
}

DEF(uint32_t)
gp0_end_tag(size_t length)
{
	return (gp0_tag(length, (void *)0xFFFFFF));
}

/*
 * GP0 drawing commands
 */

#define GP0_CMD_MISC		(0u << 29)
#define GP0_CMD_POLYGON		(1u << 29)
#define GP0_CMD_LINE		(2u << 29)
#define GP0_CMD_RECTANGLE	(3u << 29)
#define GP0_CMD_VRAM_BLIT	(4u << 29)
#define GP0_CMD_VRAM_WRITE	(5u << 29)
#define GP0_CMD_VRAM_READ	(6u << 29)
#define GP0_CMD_ATTRIBUTE	(7u << 2)

#define GP0_CMD_NOP		(GP0_CMD_MISC | (0u << 24))
#define GP0_CMD_FLUSH_CACHE	(GP0_CMD_MISC | (1u << 24))
#define GP0_CMD_VRAM_FILL	(GP0_CMD_MISC | (2u << 24))
#define GP0_CMD_NOP2		(GP0_CMD_MISC | (3u << 24))
#define GP0_CMD_IRQ		(GP0_CMD_MISC | (31u << 24)

#define GP0_CMD_TPAGE		(GP0_CMD_ATTRIBUTE | (1u << 24))
#define GP0_CMD_TWINDOW		(GP0_CMD_ATTRIBUTE | (2u << 24))
#define GP0_CMD_FB_OFFSET1	(GP0_CMD_ATTRIBUTE | (3u << 24))
#define GP0_CMD_FB_OFFSET2	(GP0_CMD_ATTRIBUTE | (4u << 24))
#define GP0_CMD_FB_ORIGIN	(GP0_CMD_ATTRIBUTE | (5u << 24))
#define GP0_CMD_FB_MASK		(GP0_CMD_ATTRIBUTE | (6u << 24)

DEF(uint32_t)
_gp0_polygon(bool quad, bool unshaded, bool gouraud, bool textured, bool blend)
{
	return (GP0_CMD_POLYGON |
		((unshaded & 1) << 24) |
		((blend & 1) << 25) |
		((textured & 1) << 26) |
		((quad & 1) << 27) |
		((gouraud & 1) << 28));
}

DEF(uint32_t)
gp0_triangle(bool textured, bool blend)
{
	return (_gp0_polygon(0, 1, 0, textured, blend));
}

DEF(uint32_t)
gp0_shaded_triangle(bool gouraud, bool textured, bool blend)
{
	return (_gp0_polygon(0, 0, gouraud, textured, blend));
}

DEF(uint32_t)
gp0_quad(bool textured, bool blend)
{
	return (_gp0_polygon(1, 1, 0, textured, blend));
}

DEF(uint32_t)
gp0_shaded_quad(bool gouraud, bool textured, bool blend)
{
	return (_gp0_polygon(1, 0, gouraud, textured, blend));
}

DEF(uint32_t)
gp0_line(bool gouraud, bool blend)
{
	return (GP0_CMD_LINE |
		((blend & 1) << 25) |
		((gouraud & 1) << 28));
}

DEF(uint32_t)
gp0_poly_line(bool gouraud, bool blend)
{
	return (GP0_CMD_LINE |
		((blend & 1) << 25) |
		(1u << 27) |
		((gouraud & 1) << 28));
}

DEF(uint32_t)
_gp0_rectangle(uint8_t size, bool textured, bool unshaded, bool blend)
{
	return (GP0_CMD_RECTANGLE |
		((unshaded & 1) << 24) |
		((blend & 1) << 25) |
		((textured & 1) << 26) |
		((size & 3) << 27);
}

DEF(uint32_t)
gp0_rectangle(bool textured, bool unshaded, bool blend)
{
	return (_gp0_rectangle(0, textured, unshaded, blend));
}

DEF(uint32_t)
gp0_rectangle1x1(bool textured, bool unshaded, bool blend)
{
	return (_gp0_rectangle(1, textured, unshaded, blend));
}

DEF(uint32_t)
gp0_rectangle8x8(bool textured, bool unshaded, bool blend)
{
	return (_gp0_rectangle(2, textured, unshaded, blend));
}

DEF(uint32_t)
gp0_rectangle16x16(bool textured, bool unshaded, bool blend)
{
	return (_gp0_rectangle(3, textured, unshaded, blend));
}


DEF(uint32_t)
gp0_vram_blit(void)
{
	return (GP0_CMD_VRAM_BLIT);
}

DEF(uint32_t)
gp0_vram_write(void)
{
	return (GP0_CMD_VRAM_WRITE);
}

DEF(uint32_t)
gp0_vram_read(void)
{
	return (GP0_CMD_VRAM_READ);
}


DEF(uint32_t)
gp0_flush_cache(void)
{
	return (GP0_CMD_FLUSH_CACHE);
}

DEF(uint32_t)
gp0_vram_fill(void)
{
	return (GP0_CMD_VRAM_FILL);
}

DEF(uint32_t)
gp0_irq(void)
{
	return (GP0_CMD_IRQ);
}


DEF(uint32_t)
gp0_set_page(uint16_t page, bool dither, bool unlock_fb) {
	return (GP0_CMD_TPAGE |
		((page & 0x9FF) << 0) |
		((dither & 1) << 9) |
		((unlock_fb & 1) << 10));
}

DEF(uint32_t)
gp0_set_window(uint8_t base_x, uint8_t base_y, uint8_t mask_x, uint8_t mask_y)
{
	return (GP0_CMD_TWINDOW |
		((mask_x & 0x1F) << 0) |
		((mask_y & 0x1F) << 5) |
		((base_x & 0x1F) << 10) |
		((base_y & 0x1F) << 15));
}

DEF(uint32_t)
gp0_fb_offset1(uint32_t x, uint32_t y)
{
	return (GP0_CMD_FB_OFFSET1 |
		((x & 0x3FF) << 0) |
		((y & 0x3FF) << 10));
}

DEF(uint32_t)
gp0_fb_offset2(uint32_t x, uint32_t y)
{
	return (GP0_CMD_FB_OFFSET2 |
		((x & 0x3FF) << 0) |
		((y & 0x3FF) << 10));
}

DEF(uint32_t)
gp0_fb_origin(int32_t x, int32_t y)
{
	return (GP0_CMD_FB_ORIGIN |
		((x & 0x7FF) <<  0) |
		((y & 0x7FF) << 11));
}

DEF(uint32_t)
gp0_fb_mask(bool set, bool use)
{
	return (GP0_CMD_FB_MASK |
		(set << 0) |
		(use << 1));
}

/*
 * GP1 display control commands
 */

#define GP1_HRES_BITMASK	((3u << 0) | (1u << 6))
#define GP1_HRES_256		(0u << 0) /* Dotclock divided by 10 */
#define GP1_HRES_320		(1u << 0) /* Dotclock divided by 8 */
#define GP1_HRES_368		(1u << 6) /* Dotclock divided by 7 */
#define GP1_HRES_512		(2u << 0) /* Dotclock divided by 5 */
#define GP1_HRES_640		(3u << 0) /* Dotclock divided by 4 */

#define GP1_VRES_BITMASK	1
#define GP1_VRES_256		0
#define GP1_VRES_512		1

#define GP1_MODE_BITMASK	1
#define GP1_MODE_NTSC		0
#define GP1_MODE_PAL		1

#define GP1_COLOR_BITMASK	1
#define GP1_COLOR_16BPP		0
#define GP1_COLOR_24BPP		1

#define GP1_DREQ_BITMASK	3
#define GP1_DREQ_NONE		0
#define GP1_DREQ_FIFO		1
#define GP1_DREQ_GP0_WRITE	2
#define GP1_DREQ_GP0_READ	3

#define GP1_VRAM_BITMASK	1
#define GP1_VRAM_1MB		0
#define GP1_VRAM_2MB		1

#define GP1_CMD_RESET_GPU	(0u << 24)
#define GP1_CMD_RESET_FIFO	(1u << 24)
#define GP1_CMD_ACKNOWLEDGE	(2u << 24)
#define GP1_CMD_DISP_BLANK	(3u << 24)
#define GP1_CMD_DREQ_MODE	(4u << 24)
#define GP1_CMD_FB_OFFSET	(5u << 24)
#define GP1_CMD_FB_RANGE_H	(6u << 24)
#define GP1_CMD_FB_RANGE_V	(7u << 24)
#define GP1_CMD_FB_MODE		(8u << 24)
#define GP1_CMD_VRAM_SIZE	(9u << 24)
#define GP1_CMD_GET_INFO	(16u << 24)

DEF(uint32_t)
gp1_clock_multiplier_h(uint32_t hres)
{
	switch (hres) {
		case GP1_HRES_256:
			return (10);

		case GP1_HRES_320:
			return (8);

		case GP1_HRES_368:
			return (7);

		case GP1_HRES_512:
			return (5);

		case GP1_HRES_640:
			return (4);

		default:
			return (0);
	}
}

DEF(uint32_t)
gp1_clock_divider_v(uint32_t vres)
{
	switch (vres) {
		case GP1_VRES_256:
			return (1);

		case GP1_VRES_512:
			return (2);

		default:
			return (0);
	}
}

DEF(uint32_t)
gp1_reset_gpu(void)
{
	return GP1_CMD_RESET_GPU;
}

DEF(uint32_t)
gp1_reset_fifo(void)
{
	return (GP1_CMD_RESET_FIFO);
}

DEF(uint32_t)
gp1_acknowledge(void)
{
	return (GP1_CMD_ACKNOWLEDGE);
}

DEF(uint32_t)
gp1_disp_blank(bool blank)
{
	return (GP1_CMD_DISP_BLANK | (blank & 1));
}

DEF(uint32_t)
gp1_dma_request_mode(uint32_t mode) {
	return (GP1_CMD_DREQ_MODE | (mode & 3));
}

DEF(uint32_t)
gp1_fb_offset(uint32_t x, uint32_t y) {
	return (GP1_CMD_FB_OFFSET |
		((x & 0x3FF) << 0) |
		((y & 0x3FF) << 10));
}

DEF(uint32_t)
gp1_fb_range_h(uint32_t low, uint32_t high) {
	return (GP1_CMD_FB_RANGE_H |
		((low & 0xFFF) << 0) |
		((high & 0xFFF) << 12);
}

DEF(uint32_t)
gp1_fb_range_v(uint32_t low, uint32_t high) {
	return (GP1_CMD_FB_RANGE_V |
		((low & 0x3FF) << 0) |
		((high & 0x3FF) << 10));
}

DEF(uint32_t)
gp1_fb_mode(uint32_t hres, uint32_t vres, uint32_t mode, bool interlace,
	uint32_t depth)
{
	return (GP1_CMD_FB_MODE |
		((hres & 0x47) << 0) |
		((vres & 1) << 2) |
		((mode & 1) << 3) |
		((depth & 1) << 4) |
		((interlace & 1) << 5));
}

DEF(uint32_t)
gp1_vram_size(uint32_t size)
{
	return (GP1_CMD_VRAM_SIZE | (size & 1));
}

#undef DEF

#endif /* _SYS__GPU_H */
