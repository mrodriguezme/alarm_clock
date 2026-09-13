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

#include "mmio.h"

enum : uintptr_t {
	SYST_CSR   = 0xE000E010,
	SYST_RVR   = 0xE000E014,
	SYST_CVR   = 0xE000E018,
	SYST_CALIB = 0xE000E01C
};

enum {
	SYST_CSR_COUNTFLAG = 1 << 16,
	SYST_CSR_CLKSOURCE = 1 << 2,
	SYST_CSR_TICKINT   = 1 << 1,
	SYST_CSR_ENABLE	   = 1 << 0
};

enum {
	SYST_RVR_RELOAD_SHIFT = 0,
	SYST_RVR_RELOAD_MASK  = (1 << 24) - 1,
};

enum {
	SYST_CVR_CURRENT_SHIFT = 0,
	SYST_CVR_CURRENT_MASK  = (1 << 24) - 1,
};

enum {
	SYST_CALIB_NOREF       = UINT32_C(1) << 31,
	SYST_CALIB_SKEW	       = 1 << 30,
	SYST_CALIB_TENMS_SHIFT = 0,
	SYST_CALIB_TENMS_MASK  = (1 << 24) - 1
};

STATIC_ALWAYS_INLINE void systick_csr_write(u32 bits)
{
	mmio_scs_write32(SYST_CSR, bits);
}

STATIC_ALWAYS_INLINE void systick_reload_val_set(u32 val)
{
	mmio_scs_write32(SYST_RVR, val);
}

STATIC_ALWAYS_INLINE void systick_curr_val_clr()
{
	mmio_scs_write32(SYST_CVR, 0);
}
