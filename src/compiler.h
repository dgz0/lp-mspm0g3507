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

/** @file compiler.h Defines wrappers around compiler specifics. */

#pragma once

/** This branch is likely to be executed. */
#define likely(x) __builtin_expect(!!(x), 1)

/** This branch is unlikely to be executed. */
#define unlikely(x) __builtin_expect(!!(x), 0)

/** This symbol is an alias for another. */
#define ALIAS(x) __attribute__((alias((x))))

/**
 * This symbol is weak; it can be overridden outside the current translation
 * unit.
 */
#define WEAK_SYMBOL __attribute__((weak))

/** This function should always be inlined under all circumstances. */
#define ALWAYS_INLINE inline __attribute__((always_inline))

/** Do not inline this function under any circumstances. */
#define NOINLINE __attribute__((noinline))

/** Always emit this function even if it does not appear to be used. */
#define FORCE_EMIT_SYMBOL __attribute__((used))

/** Place this code into a specific section. */
#define PLACE_IN_SECTION(x) __attribute__((section((x))))

/** Place this code specifically into SRAM. */
#define RAMFUNC PLACE_IN_SECTION(".ramfunc")
