// SPDX-License-Identifier: MIT
//
// Copyright 2025 dgz0
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the “Software”), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/** @file hal_startup.c Defines startup code for the LP-MSPM0G3507. */

#include <stdint.h>
#include "compiler.h"
#include "hal_asm.h"

/**
 * Defines the necessary attributes to declare an ISR.
 *
 * These attributes need not be applied outside of this translation unit.
 */
#define ISR_FUNC WEAK_SYMBOL ALIAS("hal_isr_default")

/**
 * Defines the vector table entry offsets for the MSPM0G3507.
 *
 * @note In the case of the ARMv6-M native vectors, their naming conventions and
 * definitions take place over the naming conventions and definitions provided
 * by the MSPM0Gx technical manual.
 *
 * @see MSPM0 G-Series 80-MHz Microcontrollers Technical Reference Manual
 * (Rev. B), page 600
 *
 */
enum vec_tbl_entry {
	/** The reset value of the Main stack pointer */
	VEC_TBL_ENTRY_SP_main = 0,

	/**
	 * Reset is a special form of exception that, when asserted, terminates
	 * current execution in a potentially unrecoverable way. When reset is
	 * deasserted, execution restarts from a fixed point.
	 */
	VEC_TBL_ENTRY_Reset = 1,

	/**
	 * NMI (Non Maskable Interrupt) is the highest priority exception other
	 * than reset. It is permanently enabled with a fixed priority of -2.
	 *
	 * Hardware can generate an NMI, or software can set the NMI exception
	 * to the Pending state, see Interrupt Control State Register, ICSR on
	 * page B3-229, or hardware.
	 */
	VEC_TBL_ENTRY_NMI = 2,

	/**
	 * HardFault is the generic fault that exists for all classes of fault
	 * that cannot be handled by any of the other exception mechanisms.
	 * Typically, HardFault is used for unrecoverable system failures,
	 * although this is not required, and some uses of HardFault might be
	 * recoverable. HardFault is permanently enabled with a fixed priority
	 * of -1.
	 *
	 * HardFault is used for all fault conditions on ARMv6-M.
	 */
	VEC_TBL_ENTRY_HardFault = 3,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV4 = 4,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV5 = 5,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV6 = 6,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV7 = 7,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV8 = 8,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV9 = 9,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV10 = 10,

	/**
	 * This Supervisor Call handles the exception caused by the `SVC`
	 * instruction. SVCall is permanently enabled and has a configurable
	 * priority.
	*/
	VEC_TBL_ENTRY_SVCall = 11,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV12 = 12,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV13 = 13,

	/**
	 * Used for software-generated Supervisor Calls. An application uses a
	 * Supervisor Call, if it requires servicing by the underlying operating
	 * system. The Supervisor Call associated with PendSV executes when the
	 * processor takes the PendSV interrupt.
	*/
	VEC_TBL_ENTRY_PendSV = 14,

	/**
	 * Generated by the SysTick timer and controlled using the
	 * ICSR.PENDSTSET and ICSR.PENDSTCLR bits. SysTick is an integral
	 * component of an ARMv7-M processor, and is optional in ARMv6-M.
	 *
	 * @note LP-MSPM0G3507 implements SysTick.
	 */
	VEC_TBL_ENTRY_SysTick = 15,

	/** Combined peripheral group 0 handler */
	VEC_TBL_ENTRY_INT_GROUP0 = 16,

	/** Combined peripheral group 1 handler */
	VEC_TBL_ENTRY_INT_GROUP1 = 17,

	/** Timer TIMG8 interrupt handler */
	VEC_TBL_ENTRY_TIMG8 = 18,

	/** UART3 interrupt handler */
	VEC_TBL_ENTRY_UART3 = 19,

	/** ADC0 interrupt handler */
	VEC_TBL_ENTRY_ADC0 = 20,

	/** ADC1 interrupt handler */
	VEC_TBL_ENTRY_ADC1 = 21,

	/** CAN-FD controller interrupt handler */
	VEC_TBL_ENTRY_CANFD0 = 22,

	/** DAC0 interrupt handler */
	VEC_TBL_ENTRY_DAC0 = 23,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV24 = 24,

	/** SPI0 interrupt handler */
	VEC_TBL_ENTRY_SPI0 = 25,

	/** SPI1 interrupt handler */
	VEC_TBL_ENTRY_SPI1 = 26,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV27 = 27,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV28 = 28,

	/** UART1 interrupt handler */
	VEC_TBL_ENTRY_UART1 = 29,

	/** UART2 interrupt handler */
	VEC_TBL_ENTRY_UART2 = 30,

	/** UART0 interrupt handler */
	VEC_TBL_ENTRY_UART0 = 31,

	/** Timer TIMG0 interrupt handler */
	VEC_TBL_ENTRY_TIMG0 = 32,

	/** Timer TIMG6 interrupt handler */
	VEC_TBL_ENTRY_TIMG6 = 33,

	/** Timer TIMA0 interrupt handler */
	VEC_TBL_ENTRY_TIMA0 = 34,

	/** Timer TIMA1 interrupt handler */
	VEC_TBL_ENTRY_TIMA1 = 35,

	/** Timer TIMG7 interrupt handler */
	VEC_TBL_ENTRY_TIMG7 = 36,

	/** Timer TIMG12 interrupt handler */
	VEC_TBL_ENTRY_TIMG12 = 37,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV38 = 38,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV39 = 39,

	/** I2C0 interrupt handler */
	VEC_TBL_ENTRY_I2C0 = 40,

	/** I2C1 interrupt handler */
	VEC_TBL_ENTRY_I2C1 = 41,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV42 = 42,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV43 = 43,

	/** AES accelerator interrupt handler */
	VEC_TBL_ENTRY_AES = 44,

	/** Reserved exception */
	VEC_TBL_ENTRY_RESV45 = 45,

	/** RTC interrupt handler */
	VEC_TBL_ENTRY_RTC = 46,

	/** DMA interrupt handler */
	VEC_TBL_ENTRY_DMA = 47,

	/** The total number of vector table entries */
	NUM_VEC_TBL_ENTRIES
};

/**
 * Defines the initial stack pointer at reset; generated by the linker script.
 */
extern const uint32_t HAL_STACK_PTR__;

/** Defines the newlib-nano entry point. */
extern void _start(void);

ISR_FUNC void hal_isr_NMI(void);
ISR_FUNC void hal_isr_HardFault(void);
ISR_FUNC void hal_isr_Reserved(void);
ISR_FUNC void hal_isr_SVCall(void);
ISR_FUNC void hal_isr_PendSV(void);
ISR_FUNC void hal_isr_SysTick(void);
ISR_FUNC void hal_isr_INT_GROUP0(void);
ISR_FUNC void hal_isr_INT_GROUP1(void);
ISR_FUNC void hal_isr_TIMG8(void);
ISR_FUNC void hal_isr_UART3(void);
ISR_FUNC void hal_isr_ADC0(void);
ISR_FUNC void hal_isr_ADC1(void);
ISR_FUNC void hal_isr_CANFD0(void);
ISR_FUNC void hal_isr_DAC0(void);
ISR_FUNC void hal_isr_SPI0(void);
ISR_FUNC void hal_isr_SPI1(void);
ISR_FUNC void hal_isr_UART1(void);
ISR_FUNC void hal_isr_UART2(void);
ISR_FUNC void hal_isr_UART0(void);
ISR_FUNC void hal_isr_TIMG0(void);
ISR_FUNC void hal_isr_TIMA0(void);
ISR_FUNC void hal_isr_TIMA1(void);
ISR_FUNC void hal_isr_TIMG7(void);
ISR_FUNC void hal_isr_TIMG12(void);
ISR_FUNC void hal_isr_I2C0(void);
ISR_FUNC void hal_isr_I2C1(void);
ISR_FUNC void hal_isr_AES(void);
ISR_FUNC void hal_isr_RTC(void);
ISR_FUNC void hal_isr_DMA(void);

/** Defines the interrupt handler */
NOINLINE FORCE_EMIT_SYMBOL void hal_isr_Reset(void)
{
	_start();

	// Control was returned to us...
	__BKPT();
}

/**
 * Defines the default interrupt handler.
 *
 * This interrupt handler is called when an interrupt fired but application code
 * does not implement the appropriate ISR on its own.
 */
NOINLINE FORCE_EMIT_SYMBOL void hal_isr_default(void)
{
	__BKPT();
}

PLACE_IN_SECTION(".vector_table")
void (*const vec_tbl[NUM_VEC_TBL_ENTRIES])(void) = {
	[VEC_TBL_ENTRY_SP_main]				= (void(*const)(void))&HAL_STACK_PTR__,
	[VEC_TBL_ENTRY_Reset]				= hal_isr_Reset,
	[VEC_TBL_ENTRY_NMI]				= hal_isr_NMI,
	[VEC_TBL_ENTRY_HardFault]			= hal_isr_HardFault,
	[VEC_TBL_ENTRY_RESV4 ... VEC_TBL_ENTRY_RESV10]	= hal_isr_Reserved,
	[VEC_TBL_ENTRY_SVCall]				= hal_isr_SVCall,
	[VEC_TBL_ENTRY_RESV12 ... VEC_TBL_ENTRY_RESV13]	= hal_isr_Reserved,
	[VEC_TBL_ENTRY_PendSV]				= hal_isr_PendSV,
	[VEC_TBL_ENTRY_SysTick]				= hal_isr_SysTick,
	[VEC_TBL_ENTRY_INT_GROUP0]			= hal_isr_INT_GROUP0,
	[VEC_TBL_ENTRY_INT_GROUP1]			= hal_isr_INT_GROUP1,
	[VEC_TBL_ENTRY_TIMG8]				= hal_isr_TIMG8,
	[VEC_TBL_ENTRY_UART3]				= hal_isr_UART3,
	[VEC_TBL_ENTRY_ADC0]				= hal_isr_ADC0,
	[VEC_TBL_ENTRY_ADC1]				= hal_isr_ADC1,
	[VEC_TBL_ENTRY_CANFD0]				= hal_isr_CANFD0,
	[VEC_TBL_ENTRY_DAC0]				= hal_isr_DAC0,
	[VEC_TBL_ENTRY_RESV24]				= hal_isr_Reserved,
	[VEC_TBL_ENTRY_SPI0]				= hal_isr_SPI0,
	[VEC_TBL_ENTRY_SPI1]				= hal_isr_SPI1,
	[VEC_TBL_ENTRY_RESV27 ... VEC_TBL_ENTRY_RESV28]	= hal_isr_Reserved,
	[VEC_TBL_ENTRY_UART1]				= hal_isr_UART1,
	[VEC_TBL_ENTRY_UART2]				= hal_isr_UART2,
	[VEC_TBL_ENTRY_UART0]				= hal_isr_UART0,
	[VEC_TBL_ENTRY_TIMG0]				= hal_isr_TIMG0,
	[VEC_TBL_ENTRY_TIMA0]				= hal_isr_TIMA0,
	[VEC_TBL_ENTRY_TIMA1]				= hal_isr_TIMA1,
	[VEC_TBL_ENTRY_TIMG7]				= hal_isr_TIMG7,
	[VEC_TBL_ENTRY_TIMG12]				= hal_isr_TIMG12,
	[VEC_TBL_ENTRY_RESV38 ... VEC_TBL_ENTRY_RESV39]	= hal_isr_Reserved,
	[VEC_TBL_ENTRY_I2C0]				= hal_isr_I2C0,
	[VEC_TBL_ENTRY_I2C1]				= hal_isr_I2C1,
	[VEC_TBL_ENTRY_RESV42 ... VEC_TBL_ENTRY_RESV43]	= hal_isr_Reserved,
	[VEC_TBL_ENTRY_AES]				= hal_isr_AES,
	[VEC_TBL_ENTRY_RESV45]				= hal_isr_Reserved,
	[VEC_TBL_ENTRY_RTC]				= hal_isr_RTC,
	[VEC_TBL_ENTRY_DMA]				= hal_isr_DMA
};
