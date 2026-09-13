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

#define RING_BUFFER_DEFINE(name, type, capacity)                               \
	static_assert(!((capacity) & ((capacity) - 1)),                        \
		      #name " capacity must be a power of two");               \
                                                                               \
	struct name {                                                          \
		type data[capacity];                                           \
		volatile size_t read;                                          \
		volatile size_t write;                                         \
	};                                                                     \
                                                                               \
	STATIC_ALWAYS_INLINE void name##_init(struct name *rb)                 \
	{                                                                      \
		memset(rb, 0, sizeof(*rb));                                    \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE size_t name##_mask(size_t val)      \
	{                                                                      \
		return val & ((capacity) - 1);                                 \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE size_t name##_size(struct name *rb) \
	{                                                                      \
		return rb->write - rb->read;                                   \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE bool name##_full(struct name *rb)   \
	{                                                                      \
		return name##_size(rb) == (capacity);                          \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE bool name##_empty(struct name *rb)  \
	{                                                                      \
		return rb->read == rb->write;                                  \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE type *name##_new(struct name *rb)   \
	{                                                                      \
		bool key = irq_lock();                                         \
                                                                               \
		if (unlikely(name##_full(rb)))                                 \
			rb->read++;                                            \
                                                                               \
		type *slot = &rb->data[name##_mask(rb->write++)];              \
                                                                               \
		irq_unlock(key);                                               \
		return slot;                                                   \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE type *name##_peek(struct name *rb)  \
	{                                                                      \
		return name##_empty(rb) ? NULL :                               \
					  &rb->data[name##_mask(rb->read)];    \
	}                                                                      \
                                                                               \
	[[nodiscard]] STATIC_ALWAYS_INLINE type *name##_pop(struct name *rb)   \
	{                                                                      \
		bool key = irq_lock();                                         \
                                                                               \
		type *data = name##_peek(rb);                                  \
                                                                               \
		if (data)                                                      \
			rb->read++;                                            \
                                                                               \
		irq_unlock(key);                                               \
		return data;                                                   \
	}
