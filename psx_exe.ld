/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

ENTRY(_start)

/* System RAM */
RAM_BASE = DEFINED(RAM_BASE) ? RAM_BASE : 0x80000000;
RAM_SIZE = DEFINED(RAM_SIZE) ? RAM_SIZE : 2M;

/* Application RAM */
ARAM_BASE = RAM_BASE + 64K;
ARAM_SIZE = RAM_SIZE - 64K;

MEMORY {
	/*
	 * AHDR never actually loaded in memory at ARAM_BASE - 2K, but just need
	 *   to do this trick to copy the .text section correctly with objdump
	 */
	AHDR : ORIGIN = ARAM_BASE - 2K, LENGTH = 2K
	ARAM : ORIGIN = ARAM_BASE, LENGTH = ARAM_SIZE
}

SECTIONS {
	.ahdr : {
		/* 0x00; "PS-X EXE" */
		BYTE(80); BYTE(83); BYTE(45); BYTE(88);
		BYTE(32); BYTE(69); BYTE(88); BYTE(69);

		/* 0x08; Pad / unused? */
		LONG(0);
		LONG(0);

		/* 0x10; Initial PC */
		LONG(ABSOLUTE(_start));

		/* 0x14; Initial GP */
		LONG(DEFINED(_gp) ? ABSOLUTE(_gp) : 0);

		/* 0x18; .text section */
		LONG(ARAM_BASE);
		LONG(_payload_size);

		/* 0x20; .data section */
		LONG(0);
		LONG(0);

		/* 0x28 .bss section */
		LONG(0);
		LONG(0);

		/* 0x30; Stack */
		LONG(DEFINED(_sp) ? ABSOLUTE(_sp) : ARAM_BASE + ARAM_SIZE - 0x100);
		LONG(0);

		/* Pad */
		. = ORIGIN(AHDR) + LENGTH(AHDR);
	} > AHDR = 0

	_payload_start = .;

	.text ARAM_BASE : ALIGN(4) {
		_text_start = .;

		*(.text .text.*)
		*(.gnu.linkonce.t.*)
		*(.plt)
		*(.MIPS.stubs)

		_text_end = .;
	} > ARAM

	.rodata : ALIGN(4) {
		_rodata_start = .;

		*(.rodata .rodata.*)
		*(.rdata .rdata.*)

		_rodata_end = .;
	} > ARAM

	.preinit_array : ALIGN(4) {
		_preinit_array_start = .;

		KEEP(*(.preinit_array))

		_preinit_array_end = .;
	} > ARAM

	.init_array : ALIGN(4) {
		_init_array_start = .;

		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		KEEP(*(SORT(.ctors.*)))
		KEEP(*(.ctors))

		_init_array_end = .;
	} > ARAM

	.fini_array : ALIGN(4) {
		_fini_array_start = .;

		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(SORT(.dtors.*)))
		KEEP(*(.dtors))

		_fini_array_end = .;
	} > ARAM

	.data : ALIGN(4) {
		_data_start = .; /* _data_end in .sdata */

		*(.data .data.*)
		*(.gnu.linkonce.d.*)
	} > ARAM

	.sdata : ALIGN(4) {
		_gp = ALIGN(16) + 0x7ff0;

		*(.sdata .sdata.*)
		*(.gnu.linkonce.s.*)
		*(.got.plt .got)

		_data_end = .;

		/* Linker is dumb (required for no sdata) */
		BYTE(0);

		/* Align for CD-ROM load */
		. = ALIGN(2048);
	} > ARAM = 0

	_payload_end = .;
	_payload_size = _payload_end - _payload_start;

	/* Create symbols for BSS, not a part of binary output */

	.sbss (NOLOAD) : ALIGN(4) {
		_bss_start = .;

		*(.dynsbss)
		*(.sbss .sbss.*)
		*(.gnu.linkonce.sb.*)
		*(.scommon)
	} > ARAM

	.bss (NOLOAD) : ALIGN(4) {
		*(.dynbss)
		*(.bss .bss.*)
		*(.gnu.linkonce.b.*)
		*(COMMON)

		. = ALIGN(4);
		_bss_end = .;
	} > ARAM

	/DISCARD/ : {
		*(.comment)
		*(.note.*)
		*(.eh_frame*)
		*(.MIPS.abiflags)
		*(.reginfo)
		*(.pdr)
	}
}
