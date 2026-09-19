/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/*
 * Internal
 */

struct up_ctx {
	char *buf;
	size_t buf_len;
	size_t str_len;
};

static void
up_putc(
	struct up_ctx *ctx,
	char c
	)
{
	if (ctx->buf == NULL) {
		putc(c);
	} else if (ctx->str_len + 1 < ctx->buf_len) {
		ctx->buf[ctx->str_len] = c;
	}

	ctx->str_len++;
}

static void
up_putu(
	struct up_ctx *ctx,
	uint32_t v
	)
{
	char c[10];
	int i = 0;

	if (v == 0) {
		up_putc(ctx, '0');
		return;
	}

	while (v > 0) {
		c[i++] = (char)('0' + (v % 10));
		v /= 10;
	}

	while (i > 0) {
		up_putc(ctx, c[--i]);
	}
}

void
up_puti(
	struct up_ctx *ctx,
	int32_t v
	)
{
	uint32_t uv;

	if (v < 0) {
		up_putc(ctx, '-');
		uv = -(uint32_t)v;
	} else {
		uv = (uint32_t)v;
	}

	up_putu(ctx, uv);
}

static void
up_putx(
	struct up_ctx *ctx,
	uint32_t x
	)
{
	int i;
	uint32_t p;

	if (x == 0) {
		up_putc(ctx, '0');
		return;
	}

	for (i = 0; i < 32; i += 4) {
		p = (x >> (28 - i)) & 0xF;

		if (x < (1U << (28 - i))) {
			continue;
		}

		up_putc(ctx, "0123456789ABCDEF"[p]);
	}
}

static void
up_fmt(
	struct up_ctx *ctx,
	const char *fmt,
	va_list ap
	)
{
	int c;
	const char *s;

	while ((c = *fmt++) != '\0') {
		/* Regular text */
		if (c != '%') {
			up_putc(ctx, c);
			continue;
		}

		/* Format */
		c = *fmt++;
		switch (c) {
			/* Character */
		case 'c':
			up_putc(ctx, (char)va_arg(ap, int));
			break;

			/* String */
		case 's':
			s = va_arg(ap, const char *);

			if (s == NULL) {
				s = "(null)";
			}

			while (*s != '\0') {
				up_putc(ctx, *s++);
			}

			break;

			/* Signed integer */
		case 'd':
		case 'i':
			up_puti(ctx, va_arg(ap, int32_t));
			break;

			/* Unsigned integer */
		case 'u':
			up_puti(ctx, va_arg(ap, uint32_t));
			break;

			/* Hex */
		case 'x':
		case 'X':
			up_putx(ctx, va_arg(ap, uint32_t));
			break;

			/* End */
		case '\0':
			return;

		default:
			up_putc(ctx, '%');
			up_putc(ctx, c);
			break;
		}
	}
}

/*
 * Public
 */

int
uvprintf(
	const char *fmt,
	va_list ap
	)
{
	struct up_ctx ctx;

	ctx.buf = NULL;
	ctx.buf_len = 0;
	ctx.str_len = 0;
	up_fmt(&ctx, fmt, ap);

	return ((int)ctx.str_len);
}

int
uprintf(
	const char *fmt,
	...
	)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = uvprintf(fmt, ap);
	va_end(ap);

	return (n);
}

int
uvsprintf(
	char *str,
	const char *fmt,
	va_list ap
	)
{
	struct up_ctx ctx;

	ctx.buf = str;
	ctx.buf_len = SIZE_MAX;
	ctx.str_len = 0;
	up_fmt(&ctx, fmt, ap);

	str[ctx.str_len] = '\0';

	return ((int)ctx.str_len);
}

int
usprintf(
	char *str,
	const char *fmt,
	...
	)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = uvsprintf(str, fmt, ap);
	va_end(ap);

	return (n);
}

int
uvsnprintf(
	char *str,
	size_t size,
	const char *fmt,
	va_list ap
	)
{
	struct up_ctx ctx;

	ctx.buf = str;
	ctx.buf_len = size;
	ctx.str_len = 0;
	up_fmt(&ctx, fmt, ap);

	if (size != 0) {
		str[ctx.str_len < size ? ctx.str_len : size - 1] = '\0';
	}

	return ((int)ctx.str_len);
}

int
usnprintf(
	char *str,
	size_t size,
	const char *fmt,
	...
	)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = uvsnprintf(str, size, fmt, ap);
	va_end(ap);

	return (n);
}
