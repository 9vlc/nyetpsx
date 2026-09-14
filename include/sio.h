/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

/*
 * "(not) standard (serial) io"
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <stdint.h>

void sio_init(int baud);

void putc(char c);
char getc(void);

void putl(void);
int puts(const char *str);

void putx8(uint8_t x);
void putx16(uint16_t x);
void putx32(uint32_t x);

void putp(const void *ptr);

void putu32(uint32_t v);
void puti32(int32_t v);

#endif /* _STDIO_H */
