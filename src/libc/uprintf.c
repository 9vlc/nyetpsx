/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
*/

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/*
 * Internal
 */

#define UP_FLAG_ZPAD	(1u << 0) /* 0, Pad with zeroes */
#define UP_FLAG_SPAD	(1u << 1) /*  , Pad with spaces */
#define UP_FLAG_LEFT	(1u << 2) /* -, Left justify */
#define UP_FLAG_PLUS	(1u << 3) /* +, Print plus sign */
#define UP_FLAG_PREFIX	(1u << 4) /* #, Print prefix (0x, 0b, ...) */
#define UP_FLAG_UPPER	(1u << 5) /* Uppercase prefix */
#define UP_FLAG_SIGNED	(1u << 6) /* Number is signed */
typedef uint32_t up_flags_t;

struct up_ctx {
	int (*putchar)(int c);
	char *buf;
	size_t buf_len;
	size_t str_len;
};
typedef struct up_ctx up_ctx_t;

static void
up_putchar(
	up_ctx_t *ctx,
	int c
	)
{
	if (ctx->putchar) {
		ctx->putchar(c);
	} else if (ctx->buf != NULL && ctx->str_len + 1 < ctx->buf_len) {
		ctx->buf[ctx->str_len] = c;
	}

	ctx->str_len++;
}

static void
up_pad(
	up_ctx_t *ctx,
	int c,
	int n
	)
{
	while (n-- > 0) {
		up_putchar(ctx, c);
	}
}

static void
up_putnum(
	up_ctx_t *ctx,
	up_flags_t flags,
	int width,
	int base,
	uint32_t v
	)
{
	char buf[64 + 1 + 2]; /* 64 digits + sign + prefix */
	int len = 0;
	int hlen;
	int i, pad;

	const char * const digits = (flags & UP_FLAG_UPPER ?
		"0123456789ABCDEF" :
		"0123456789abcdef");

	/* Sign */
	if (flags & UP_FLAG_SIGNED) {
		int32_t sv = (int32_t)v;

		if (sv < 0) {
			buf[len++] = '-';
			v = -(uint32_t)sv;
		} else if (flags & UP_FLAG_PLUS) {
			buf[len++] = '+';
		} else if (flags & UP_FLAG_SPAD) {
			buf[len++] = ' ';
		}
	}

	/* Prefix */
	if ((flags & UP_FLAG_PREFIX) && base != 10) {
		buf[len++] = '0';

		if (base == 16) {
			buf[len++] = (flags & UP_FLAG_UPPER ? 'X' : 'x');
		} else if (base == 2) {
			buf[len++] = (flags & UP_FLAG_UPPER ? 'B' : 'b');
		}
	}

	hlen = len;

	/* Number */
	do {
		buf[len++] = digits[v % base];
		v /= base;
	} while (v > 0);

	pad = width - len;

	/* Space pad (right justify) */
	if (!(flags & (UP_FLAG_LEFT | UP_FLAG_ZPAD))) {
		up_pad(ctx, ' ', pad);
	}

	/* Sign and prefix */
	for (i = 0; i < hlen; i++) {
		up_putchar(ctx, buf[i]);
	}

	/* Zero pad */
	if ((flags & UP_FLAG_ZPAD) && !(flags & UP_FLAG_LEFT)) {
		up_pad(ctx, '0', pad);
	}

	/* Digits */
	while (len > hlen) {
		up_putchar(ctx, buf[--len]);
	}

	/* Space pad (left justify) */
	if (flags & UP_FLAG_LEFT) {
		up_pad(ctx, ' ', pad);
	}
}

static void
up_fmt(
	up_ctx_t *ctx,
	const char *fmt,
	va_list ap
	)
{
	int c, width;
	up_flags_t flags;

	while ((c = *fmt++) != '\0') {
		/* Regular text */
		if (c != '%') {
			up_putchar(ctx, c);
			continue;
		}

		/* Formatted text: The Movie */

		flags = 0;
		width = 0;

		/* Flags */
		while (1) {
			c = *fmt++;
			if (c == '0') {
				flags |= UP_FLAG_ZPAD;
			} else if (c == ' ') {
				flags |= UP_FLAG_SPAD;
			} else if (c == '-') {
				flags |= UP_FLAG_LEFT;
			} else if (c == '+') {
				flags |= UP_FLAG_PLUS;
			} else if (c == '#') {
				flags |= UP_FLAG_PREFIX;
			} else {
				break;
			}
		}

		/* Width */
		if (c == '*') {
			width = va_arg(ap, int);
			if (width < 0) {
				flags |= UP_FLAG_LEFT;
				width = -width;
			}
			c = *fmt++;
		} else {
			/* TODO: Replace this with strtol or atoi or idk?? */
			while (c >= '0' && c <= '9') {
				width = width * 10 + c - '0';
				c = *fmt++;
			}
		}

		switch (c) {

			/* Character */
		case 'c':
			if (!(flags & UP_FLAG_LEFT)) {
				up_pad(ctx, ' ', width - 1);
			}

			up_putchar(ctx, va_arg(ap, int));

			if ((flags & UP_FLAG_LEFT) && width > 1) {
				up_pad(ctx, ' ', width - 1);
			}

			break;

			/* String */
		case 's': {
			const char *s = va_arg(ap, const char *);
			size_t slen;
			int pad;

			if (s == NULL) {
				s = "(null)";
			}

			slen = 0;
			while (s[slen] != '\0') {
				slen++;
			}

			pad = (width > (int)slen ? width - (int)slen : 0);

			if (!(flags & UP_FLAG_LEFT)) {
				up_pad(ctx, ' ', pad);
			}

			while (*s != '\0') {
				up_putchar(ctx, *s++);
			}

			if (flags & UP_FLAG_LEFT) {
				up_pad(ctx, ' ', pad);
			}

			break;
		}

			/* Int */
		case 'd':
		case 'i':
			up_putnum(ctx, flags | UP_FLAG_SIGNED, width, 10,
				va_arg(ap, uint32_t));
			break;

			/* UInt */
		case 'u':
			up_putnum(ctx, flags, width, 10,
				va_arg(ap, uint32_t));
			break;

			/* Octal */
		case 'o':
			up_putnum(ctx, flags, width, 8,
				va_arg(ap, uint32_t));
			break;

			/* Binary */
		case 'B':
			flags |= UP_FLAG_UPPER;
		case 'b':
			up_putnum(ctx, flags, width, 2,
				va_arg(ap, uint32_t));
			break;

			/* Hex */
		case 'X':
			flags |= UP_FLAG_UPPER;
		case 'x':
			up_putnum(ctx, flags, width, 16,
				va_arg(ap, uint32_t));
			break;

			/* Pointer */
		case 'p': {
			up_putnum(ctx, flags | UP_FLAG_PREFIX, width, 16,
				(uintptr_t)va_arg(ap, void *));
			break;
		}

			/* "%%" */
		case '%':
			up_putchar(ctx, '%');
			break;

			/* End */
		case '\0':
			return;

		default:
			up_putchar(ctx, '%');
			up_putchar(ctx, c);
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
	up_ctx_t ctx;

	ctx.putchar = putchar;
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
uvcprintf(
	int (*putchar)(int c),
	const char *fmt,
	va_list ap
	)
{
	up_ctx_t ctx;

	ctx.putchar = putchar;
	ctx.buf_len = 0;
	ctx.str_len = 0;
	up_fmt(&ctx, fmt, ap);

	return ((int)ctx.str_len);
}

int
ucprintf(
	int (*putchar)(int c),
	const char *fmt,
	...
	)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = uvcprintf(putchar, fmt, ap);
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
	up_ctx_t ctx;

	ctx.putchar = NULL;
	ctx.buf     = str;
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
	up_ctx_t ctx;

	ctx.putchar = NULL;
	ctx.buf     = str;
	ctx.buf_len = size;
	ctx.str_len = 0;
	up_fmt(&ctx, fmt, ap);

	if (str != NULL && size != 0) {
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
