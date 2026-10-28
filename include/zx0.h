/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _ZX0_H
#define _ZX0_H

#include <stdbool.h>
#include <stdint.h>

enum zx0_state {
	ZX0_LIT_LEN = 0,
	ZX0_LIT_DATA,
	ZX0_AFTER_LIT,
	ZX0_REP_LEN,
	ZX0_AFTER_MATCH,
	ZX0_OFS_MSB,
	ZX0_OFS_LSB,
	ZX0_MATCH_LEN,
	ZX0_DONE
};

struct zx0_ctx {
	int state;

	/* Last match offset */
	uint32_t m_off;

	/* Gamma accumulator */
	uint32_t g_acm;

	/* Next gamma is data */
	bool gdata;

	/* Gotta go fast */
	uint_fast8_t bits, mask;
};
typedef struct zx0_ctx zx0_ctx_t;

void
zx0_init(
	zx0_ctx_t *ctx
	);

int
zx0_dec(
	zx0_ctx_t *ctx,
	uint8_t b,
	uint8_t **dst
	);

#endif /* _ZX0_H */
