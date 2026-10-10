# ps1-bare-metal - (C) 2023-2025 spicyjpeg
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

.set noreorder

# This is a minimal implementation of a function to flush the CPU's instruction
# cache. While a similar function is already present in the BIOS ROM, this
# version is several orders of magnitude faster as it only clears the tags (the
# BIOS implementation unnecessarily zeroes out the contents of every cache line)
# and is not subject to the waitstates imposed by the ROM's 8-bit bus.

.set KSEG1_BASE, 0xA0000000
.set CPU_BCC,    0xFFFE0130

.set CPU_BCC_TAG, 1 <<  2
.set CPU_BCC_DS,  1 <<  7
.set CPU_BCC_IS1, 1 << 11

.set COP0_STATUS, $12

.set COP0_STATUS_IsC, 1 << 16

.set ptr,		$a0
.set temp,		$a1
.set saved_status,	$a2
.set saved_bcc,		$a3

.section .text.cpu_flush_cache, "ax", @progbits
.global cpu_flush_cache
.type cpu_flush_cache, @function

cpu_flush_cache:
	# Call _flushCacheInner() through the uncached KSEG1 mirror of main RAM,
	# ensuring the CPU will not attempt to use the cache while it is being
	# cleared. This jump must be performed using a la/jr combo as immediate
	# jumps (j) only update the program counter's bottommost 28 bits.
	la    ptr, _cpu_flush_cache_inner
	lui   temp, %hi(KSEG1_BASE)
	or    ptr, temp

	jr    ptr
	lui   ptr, %hi(CPU_BCC)

.section .text._flush_cache_inner, "ax", @progbits
.type _flush_cache_nner, @function

_cpu_flush_cache_inner:
	# Save the current state of the BCC and COP0 status registers so that they
	# can be restored later.
	mfc0  saved_status, COP0_STATUS
	lw    saved_bcc, %lo(CPU_BCC)(ptr)

	# Disable interrupts and the scratchpad, put the instruction cache into "tag
	# test" mode and proceed to map it directly to the CPU's address space by
	# setting the COP0 "isolate cache" flag.
	mtc0  $0, COP0_STATUS

	# CPU_BCC = (CPU_BCC & ~CPU_BCC_DS) | CPU_BCC_TAG | CPU_BCC_IS1;
	li    temp, ~CPU_BCC_DS
	and   temp, saved_bcc
	ori   temp, CPU_BCC_TAG | CPU_BCC_IS1
	sw    temp, %lo(CPU_BCC)(ptr)

	# cop0_setReg(COP0_STATUS, COP0_STATUS_IsC);
	li    temp, COP0_STATUS_IsC
	mtc0  temp, COP0_STATUS

	# Use an unrolled loop to clear all tags, thus invalidating the cache's
	# contents. "Tag test" mode maps each tag into memory at the offset of its
	# respective 16-byte cache line.
	li    temp, 0x1000 - 256

.L: # for (int i = 0x1000 - 256; i >= 0; i -= 256) {
	sw    $0, 0x00(temp)
	sw    $0, 0x10(temp)
	sw    $0, 0x20(temp)
	sw    $0, 0x30(temp)
	sw    $0, 0x40(temp)
	sw    $0, 0x50(temp)
	sw    $0, 0x60(temp)
	sw    $0, 0x70(temp)
	sw    $0, 0x80(temp)
	sw    $0, 0x90(temp)
	sw    $0, 0xA0(temp)
	sw    $0, 0xB0(temp)
	sw    $0, 0xC0(temp)
	sw    $0, 0xD0(temp)
	sw    $0, 0xE0(temp)
	sw    $0, 0xF0(temp)

	# }
	bgtz  temp, .L
	addiu temp, -256

	# Clear the "isolate cache" bit, restore the previously saved state and
	# return.
	mtc0  $0, COP0_STATUS
	nop
	sw    saved_bcc, %lo(CPU_BCC)(ptr)
	mtc0  saved_status, COP0_STATUS

	jr    $ra
	nop
