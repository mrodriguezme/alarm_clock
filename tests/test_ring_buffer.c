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

#include <string.h>

#include "unity.h"
#include "common/compiler.h"
#include "board/ring_buffer.h"

static bool irq_lock(void)
{
	return false;
}

static void irq_unlock(bool key)
{
	(void)key;
}

// Small capacity (must be power of two) so overwrite/wraparound cases are easy
// to hit without huge loops.
RING_BUFFER_DEFINE(rb4, int, 4);

static struct rb4 rb;

void setUp(void)
{
	rb4_init(&rb);
}

void tearDown(void)
{
}

void test_new_buffer_is_empty(void)
{
	TEST_ASSERT_TRUE(rb4_empty(&rb));
	TEST_ASSERT_FALSE(rb4_full(&rb));
	TEST_ASSERT_EQUAL_UINT(0, rb4_size(&rb));
	TEST_ASSERT_NULL(rb4_peek(&rb));
	TEST_ASSERT_NULL(rb4_pop(&rb));
}

void test_push_one_and_pop_one(void)
{
	int *slot = rb4_new(&rb);

	TEST_ASSERT_NOT_NULL(slot);
	*slot = 42;

	TEST_ASSERT_FALSE(rb4_empty(&rb));
	TEST_ASSERT_EQUAL_UINT(1, rb4_size(&rb));

	int *peeked = rb4_peek(&rb);

	TEST_ASSERT_NOT_NULL(peeked);
	TEST_ASSERT_EQUAL_INT(42, *peeked);

	int *popped = rb4_pop(&rb);

	TEST_ASSERT_NOT_NULL(popped);
	TEST_ASSERT_EQUAL_INT(42, *popped);
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_fill_to_capacity(void)
{
	for (int i = 0; i < 4; ++i) {
		int *slot = rb4_new(&rb);

		TEST_ASSERT_NOT_NULL(slot);
		*slot = i;
	}

	TEST_ASSERT_TRUE(rb4_full(&rb));
	TEST_ASSERT_EQUAL_UINT(4, rb4_size(&rb));

	for (int i = 0; i < 4; ++i) {
		int *popped = rb4_pop(&rb);

		TEST_ASSERT_NOT_NULL(popped);
		TEST_ASSERT_EQUAL_INT(i, *popped);
	}
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_overwrite_oldest_when_full(void)
{
	// Fill the buffer: 0, 1, 2, 3
	for (int i = 0; i < 4; ++i)
		*rb4_new(&rb) = i;

	TEST_ASSERT_TRUE(rb4_full(&rb));

	// Buffer is full - pushing again should silently drop the oldest
	// element (0) to make room for the new one.
	*rb4_new(&rb) = 99;

	TEST_ASSERT_TRUE(rb4_full(&rb));
	TEST_ASSERT_EQUAL_UINT(4, rb4_size(&rb));

	// Expect 1, 2, 3, 99 back; '0' was overwritten.
	int expected[] = { 1, 2, 3, 99 };

	for (int i = 0; i < 4; ++i) {
		int *popped = rb4_pop(&rb);

		TEST_ASSERT_NOT_NULL(popped);
		TEST_ASSERT_EQUAL_INT(expected[i], *popped);
	}
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_repeated_overwrite_keeps_only_newest_capacity_elements(void)
{
	// Push far more than capacity in one burst; buffer should end up
	// holding exactly the last 4 values pushed, in order.
	for (int i = 0; i < 10; ++i)
		*rb4_new(&rb) = i;

	TEST_ASSERT_TRUE(rb4_full(&rb));

	int expected[] = { 6, 7, 8, 9 };

	for (int i = 0; i < 4; ++i) {
		int *popped = rb4_pop(&rb);

		TEST_ASSERT_NOT_NULL(popped);
		TEST_ASSERT_EQUAL_INT(expected[i], *popped);
	}
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_wraparound_indices(void)
{
	// Push/pop below capacity repeatedly so read/write indices march well
	// past the buffer's physical size, exercising the masking logic over
	// many wraps.
	for (int round = 0; round < 100; ++round) {
		for (int i = 0; i < 3; ++i)
			*rb4_new(&rb) = (round * 3) + i;

		for (int i = 0; i < 3; ++i) {
			int *popped = rb4_pop(&rb);

			TEST_ASSERT_NOT_NULL(popped);
			TEST_ASSERT_EQUAL_INT((round * 3) + i, *popped);
		}
	}
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_peek_does_not_consume(void)
{
	*rb4_new(&rb) = 7;

	int *peek1 = rb4_peek(&rb);
	int *peek2 = rb4_peek(&rb);

	TEST_ASSERT_EQUAL_PTR(peek1, peek2);
	TEST_ASSERT_EQUAL_UINT(1, rb4_size(&rb));

	(void)rb4_pop(&rb);
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}

void test_pop_on_empty_returns_null_repeatedly(void)
{
	TEST_ASSERT_NULL(rb4_pop(&rb));
	TEST_ASSERT_NULL(rb4_pop(&rb));
	TEST_ASSERT_TRUE(rb4_empty(&rb));
}
