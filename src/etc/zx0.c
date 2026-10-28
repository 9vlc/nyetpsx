/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <stdbool.h>
#include <stdint.h>
#include <zx0.h>

void
zx0_init(
	zx0_ctx_t *ctx
	)
{
	ctx->m_off = 1;
	ctx->g_acm = 1;
	ctx->state = 0;
	ctx->bits  = 0;
	ctx->mask  = 0;
	ctx->gdata = true;
}

int
zx0_dec(
	zx0_ctx_t *ctx,
	uint8_t b,
	uint8_t **dst
	)
{
	uint8_t *d = *dst;
	uint32_t bit = 0;
	uint32_t n;
	bool bit_pending = false;

	switch (ctx->state) {
	case ZX0_DONE:
		return (-1);

	case ZX0_LIT_DATA:
		*d++ = b;
		if (--ctx->g_acm != 0) {
			goto end;
		}
		ctx->state = ZX0_AFTER_LIT;
		break;

	case ZX0_OFS_LSB:
		ctx->m_off = (ctx->g_acm << 7) - (b >> 1);
		ctx->g_acm = 1;
		ctx->state = ZX0_MATCH_LEN;
		bit = b & 1;
		bit_pending = 1;
		break;

	default:
		ctx->bits = b;
		ctx->mask = 0x80;
		break;
	}

	while (1) {
		if (!bit_pending) {
			if (ctx->mask == 0) {
				break;
			}
			bit = (ctx->bits & ctx->mask) != 0;
			ctx->mask >>= 1;
		}
		bit_pending = 0;

		if (ctx->state == ZX0_AFTER_LIT) {
			ctx->state = (bit ? ZX0_OFS_MSB : ZX0_REP_LEN);
			ctx->g_acm = 1;
			continue;
		}

		if (ctx->state == ZX0_AFTER_MATCH) {
			ctx->state = (bit ? ZX0_OFS_MSB : ZX0_LIT_LEN);
			ctx->g_acm = 1;
			continue;
		}

		if (ctx->gdata) {
			if (ctx->state == ZX0_OFS_MSB) {
				bit ^= 1;
			}
			ctx->g_acm = (ctx->g_acm << 1) | bit;
			ctx->gdata = 0;
			continue;
		}

		if (bit == 0) {
			ctx->gdata = 1;
			continue;
		}

		if (ctx->state == ZX0_LIT_LEN) {
			ctx->state = ZX0_LIT_DATA;
			goto end;
		}

		if (ctx->state == ZX0_OFS_MSB) {
			if (ctx->g_acm == 256) {
				ctx->state = ZX0_DONE;
				*dst = d;
				return (0);
			}
			ctx->state = ZX0_OFS_LSB;
			goto end;
		}

		n = ctx->g_acm + (ctx->state == ZX0_MATCH_LEN ? 1 : 0);
		do {
			*d = *(d - ctx->m_off);
			d++;
		} while (--n != 0);
		ctx->state = ZX0_AFTER_MATCH;
	}

end:
	*dst = d;
	return (1);
}
