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

#pragma once

#include "common/compiler.h"
#include "common/types.h"

STATIC_ALWAYS_INLINE void nop()
{
	asm volatile("nop" ::: "memory");
}

[[nodiscard]] STATIC_ALWAYS_INLINE bool irq_lock()
{
	u32 primask;

	asm volatile("mrs %0, primask" : "=r"(primask));

	if (likely(!primask))
		asm volatile("cpsid i" ::: "memory");

	return primask;
}

STATIC_ALWAYS_INLINE void irq_unlock(bool key)
{
	if (likely(!key))
		asm volatile("cpsie i" ::: "memory");
}

STATIC_ALWAYS_INLINE void dsb()
{
	asm volatile("dsb" ::: "memory");
}

STATIC_ALWAYS_INLINE void isb()
{
	asm volatile("isb" ::: "memory");
}
