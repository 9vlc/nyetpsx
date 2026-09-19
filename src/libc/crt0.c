/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

typedef void (*vfunc)(void);

/*
 * These are defined by the linker script. Note that these are not variables,
 *   they are virtual symbols whose location matches their value.
 * The simplest way to turn them into pointers is to declare them as arrays.
 */
extern unsigned char _bss_start[], _bss_end[];

extern const vfunc _preinit_array_start[], _preinit_array_end[];
extern const vfunc _init_array_start[], _init_array_end[];
extern const vfunc _fini_array_start[], _fini_array_end[];

/*
 * Program entry point
 */
int main(int argc, const char **argv);

int
_start(
	int argc,
	const char **argv
	)
{
	/*
	 * Set $gp to point to the middle of the .sdata/.sbss sections, ensuring
	 *   variables placed in those sections can be quickly accessed.
	 * See the linker script for more details.
	 */
	__asm__ __volatile__("la $gp, _gp\n");

	/*
	 * Set all uninitialized variables to zero by clearing the BSS section.
	 * Slow, not pretty, but does not rely on anything external.
	 */
	for (unsigned char *p = _bss_start; p < _bss_end; p++) {
		*p = 0;
	}

	/*
	 * Invoke all global constructors if any, then main() and finally
	 *   all global destructors.
	 */
	for (const vfunc *f = _preinit_array_start; f < _preinit_array_end; f++) {

		(*f)();
	}

	for (const vfunc *f = _init_array_start; f < _init_array_end; f++) {
		(*f)();
	}

	int ret = main(argc, argv);

	for (const vfunc *f = _fini_array_start; f < _fini_array_end; f++) {
		(*f)();
	}

	return (ret);
}
