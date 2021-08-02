// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CTEST_WINDOWS_H
#define CTEST_WINDOWS_H

#include "macro_utils/macro_utils.h"

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#include "windows.h"

#ifdef __cplusplus
extern "C" {
#endif

void ULONG64_AssertAreEqual(ULONG64 left, ULONG64 right, char* ctest_message, int line_no);
void ULONG_AssertAreEqual(ULONG left, ULONG right, char* ctest_message, int line_no);
void LONG_AssertAreEqual(LONG left, LONG right, char* ctest_message, int line_no);
void LONG64_AssertAreEqual(LONG64 left, LONG64 right, char* ctest_message, int line_no);
void HRESULT_AssertAreEqual(HRESULT left, HRESULT right, char* ctest_message, int line_no);

void ULONG64_AssertAreNotEqual(ULONG64 left, ULONG64 right, char* ctest_message, int line_no);
void ULONG_AssertAreNotEqual(ULONG left, ULONG right, char* ctest_message, int line_no);
void LONG_AssertAreNotEqual(LONG left, LONG right, char* ctest_message, int line_no);
void LONG64_AssertAreNotEqual(LONG64 left, LONG64 right, char* ctest_message, int line_no);
void HRESULT_AssertAreNotEqual(HRESULT left, HRESULT right, char* ctest_message, int line_no);

#ifdef __cplusplus
}
#endif

#endif // CTEST_WINDOWS_H
