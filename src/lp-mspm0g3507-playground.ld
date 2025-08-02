/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2025 dgz0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* How large should the stack be? */
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 1K;

/* What is the entry point for the firmware? */
ENTRY(hal_isr_Reset);

MEMORY
{
	/*
	 * The code region contains the flash memory used to store executable
	 * code and data. Accesses to the flash memory from the CPU through the
	 * code region are processed through the AHB bus matrix to the flash
	 * read interface directly. The flash memory is aliased to two address
	 * spaces in the code region: one returning ECC-corrected data, the
	 * other providing uncorrected (raw) data.
	 *
	 * The code region also contains the read-only memory (ROM) used for the
	 * TI device boot code and the bootstrap loader. The ROM is only
	 * available during the initial device boot process.
	 *
	 * See section 7.2.3.1 on page 753 of the MSPM0 G-Series 80-MHz
	 * Microcontrollers Technical Reference Manual (Rev. B) for the detailed
	 * flash memory map.
	 *
	 * The datasheet for the MSPM0G3507 defines the ECC corrected memory
	 * region as 0x0000'0000 to 0x0001'FFF8; see MSPM0G350x Mixed-Signal
	 * Microcontrollers With CAN-FD Interface datasheet (Rev. B) page 55.
	 */
	 FLASH(rx) : ORIGIN = 0x00000000, LENGTH = 0x0001FFF8

	 /*
	  * The SRAM region contains the system memory (SRAM). The SRAM supports
	  * zero wait state access at the maximum MCLK frequency (80MHz).
	  * Accesses to the SRAM from the CPU are processed through the AHB bus
	  * matrix to the SRAM interface directly.
	  *
	  * The datasheet for the MSPM0G3507 defines the parity checked memory
	  * region as 0x2010'0000 to 0x2010'7FFF; see MSPM0G350x Mixed-Signal
          * Microcontrollers With CAN-FD Interface datasheet (Rev. B) page 55.
	  *
	  * Code in SRAM is executable by default; remove the executable flag
	  * (x) to prevent this.
	  */
	  SRAM(rwx) : ORIGIN = 0x20100000, LENGTH = 0x00007FFF
}

SECTIONS
{
	.text : {
		KEEP (*(.vector_table))
		KEEP (*(.text))
		. = ALIGN(0x8);
		*(.text.*)
		. = ALIGN(0x8);
		KEEP (*(.ctors))
		. = ALIGN(0x8);
		KEEP (*(.dtors))
		. = ALIGN(0x8);
		KEEP (*(.init))
		. = ALIGN(0x8);
		KEEP (*(.fini*))
		. = ALIGN(0x8);
	} > FLASH AT> FLASH

	.rodata : {
		*(.rodata)
		. = ALIGN(8);
		*(.rodata.*)
		. = ALIGN(8);
	} > SRAM AT> FLASH

	.data : {
		__HAL_DATA_SECT_LOAD_ADDR__ = LOADADDR(.data);
		__HAL_DATA_SECT_START_ADDR__ = .;
		KEEP(*(.data))
		KEEP(*(.data*))
		. = ALIGN(8);
		__HAL_DATA_SECT_END_ADDR__ = .;
	} > SRAM AT> FLASH

	.bss : {
		__bss_start__ = .;
		KEEP(*(.bss))
		*(.bss.*)
		*(COMMON)
		. = ALIGN(8);
		__bss_end__ = .;
	} > SRAM AT> SRAM

	.stack (NOLOAD) : ALIGN(8) {
		_stack = .;
		KEEP(*(.stack))
	} > SRAM AT> SRAM

	HAL_STACK_PTR__ = ORIGIN(SRAM) + LENGTH(SRAM);
	PROVIDE(__stack = HAL_STACK_PTR__);
}
