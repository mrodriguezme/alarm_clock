// SPDX-License-Identifier: MIT
//
// Copyright 2026 Michael Rodriguez
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <stddef.h>
#include "intrinsics.h"

extern void _start();
extern u32 __stack;

constexpr size_t VEC_EXT_INT = 16;

enum : size_t {
	VEC_SP	       = 0,
	VEC_RST	       = 1,
	VEC_NMI	       = 2,
	VEC_HARDFAULT  = 3,
	VEC_SVCALL     = 11,
	VEC_PENDSV     = 14,
	VEC_SYSTICK    = 15,
	VEC_INT_GROUP0 = VEC_EXT_INT + 0,
	VEC_GPIO0      = VEC_EXT_INT + 1,
	VEC_TIMG8      = VEC_EXT_INT + 2,
	VEC_ADC	       = VEC_EXT_INT + 4,
	VEC_SPI0       = VEC_EXT_INT + 9,
	VEC_UART0      = VEC_EXT_INT + 15,
	VEC_TIMG14     = VEC_EXT_INT + 16,
	VEC_TIMA0      = VEC_EXT_INT + 18,
	VEC_I2C0       = VEC_EXT_INT + 24,
	VEC_DMA	       = VEC_EXT_INT + 31
};

#define ISR(fn) \
	NOINLINE USED WEAK_SYMBOL SYMBOL_ALIAS("isr_unhandled") void fn()

ISR(isr_nmi);
ISR(isr_hardfault);
ISR(isr_svcall);
ISR(isr_pendsv);
ISR(isr_systick);
ISR(isr_int_group0);
ISR(isr_gpio0);
ISR(isr_adc);
ISR(isr_spi0);
ISR(isr_uart0);
ISR(isr_timg14);
ISR(isr_tima0);
ISR(isr_i2c0);
ISR(isr_dma);

#undef ISR

void isr_unhandled()
{
	for (;;)
		nop();
}

PLACE_IN_SECTION(".init")
USED const void *const __interrupt_vector[] = {
	// clang-format off

	[VEC_SP]		= &__stack,
	[VEC_RST]		= _start,
	[VEC_NMI]		= isr_nmi,
	[VEC_HARDFAULT]		= isr_hardfault,
	[VEC_SVCALL]		= isr_svcall,
	[VEC_PENDSV]		= isr_pendsv,
	[VEC_SYSTICK]		= isr_systick,
	[VEC_INT_GROUP0]	= isr_int_group0,
	[VEC_GPIO0]		= isr_gpio0,
	[VEC_ADC]		= isr_adc,
	[VEC_SPI0]		= isr_spi0,
	[VEC_UART0]		= isr_uart0,
	[VEC_TIMG14]		= isr_timg14,
	[VEC_TIMA0]		= isr_tima0,
	[VEC_I2C0]		= isr_i2c0,
	[VEC_DMA]		= isr_dma

	// clang-format on
};
