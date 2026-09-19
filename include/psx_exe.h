/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _PSX_EXE_H
#define _PSX_EXE_H

#include <stdint.h>

struct psx_exe_param {
	/* 0x10; Initial PC */
	uint32_t pc;

	/* 0x14; Initial GP ($28) */
	uint32_t gp;

	/* 0x18; .text section; Payload copied here */
	uint32_t text_addr;
	uint32_t text_size;

	/* 0x20; .data section; Global vars, unused in older compilers ? */
	uint32_t data_addr;
	uint32_t data_size;

	/* 0x28; .bss section; Filled with zeroes */
	uint32_t bss_addr;
	uint32_t bss_size;

	/* 0x30; Stack */
	uint32_t stack_addr;
	uint32_t stack_size;

	/* 0x38; Reserved by kernel */
	uint32_t _reserved1[4];

	/* 0x4C */
};

struct psx_exe_hdr {
	/* 0x00; "PS-X EXE" */
	uint8_t magic[0x08];

	/* 0x08; Unused ? */
	uint8_t _unused1[0x08];

	/* 0x10; Parameters spanning 0x60 bytes */
	struct psx_exe_param param;

	/* 0x4C; License string and padding */
	uint8_t license[0x7B4];

	/* 0x800; Payload start */
};

#endif /* _PSX_EXE_H */
