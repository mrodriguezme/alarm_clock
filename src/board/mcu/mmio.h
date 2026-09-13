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

#include "common/util.h"
#include "intrinsics.h"

#define DEFINE_MMIO_HELPERS(bitwidth)                                        \
	STATIC_ALWAYS_INLINE u##bitwidth mmio_read##bitwidth(uintptr_t addr) \
	{                                                                    \
		return *((volatile u##bitwidth *)addr);                      \
	}                                                                    \
                                                                             \
	STATIC_ALWAYS_INLINE void mmio_write##bitwidth(uintptr_t addr,       \
						       u##bitwidth val)      \
	{                                                                    \
		*((volatile u##bitwidth *)addr) = val;                       \
	}                                                                    \
                                                                             \
	STATIC_ALWAYS_INLINE void mmio_set##bitwidth(uintptr_t addr,         \
						     u##bitwidth bits)       \
	{                                                                    \
		u##bitwidth val = mmio_read##bitwidth(addr);                 \
		mmio_write##bitwidth(addr, val | bits);                      \
	}                                                                    \
                                                                             \
	STATIC_ALWAYS_INLINE void mmio_clr##bitwidth(uintptr_t addr,         \
						     u##bitwidth bits)       \
	{                                                                    \
		u##bitwidth val = mmio_read##bitwidth(addr);                 \
		mmio_write##bitwidth(addr, val & ~bits);                     \
	}                                                                    \
                                                                             \
	STATIC_ALWAYS_INLINE void mmio_set_mask##bitwidth(uintptr_t addr,    \
							  u##bitwidth mask,  \
							  u##bitwidth shift, \
							  u##bitwidth val)   \
	{                                                                    \
		u##bitwidth dst = mmio_read##bitwidth(addr);                 \
		set_val_by_mask(dst, mask, shift, val);                      \
		mmio_write##bitwidth(addr, dst);                             \
	}

DEFINE_MMIO_HELPERS(32);

#undef DEFINE_MMIO_HELPERS

STATIC_ALWAYS_INLINE u32 mmio_scs_read32(uintptr_t addr)
{
	return mmio_read32(addr);
}

STATIC_ALWAYS_INLINE void mmio_scs_write32(uintptr_t addr, u32 val)
{
	mmio_write32(addr, val);
	dsb();
	isb();
}

STATIC_ALWAYS_INLINE void mmio_scs_set32(uintptr_t addr, u32 bits)
{
	u32 data = mmio_read32(addr);
	mmio_scs_write32(addr, data | bits);
}

STATIC_ALWAYS_INLINE void mmio_scs_clr32(uintptr_t addr, u32 bits)
{
	u32 data = mmio_read32(addr);
	mmio_scs_write32(addr, data & ~bits);
}

STATIC_ALWAYS_INLINE void mmio_scs_set_mask32(uintptr_t addr, u32 mask,
					      u32 shift, u32 val)
{
	u32 data = mmio_read32(addr);
	set_val_by_mask(data, mask, shift, val);
	mmio_scs_write32(addr, data);
}
