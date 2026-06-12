// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// Same leak as the /MD DLL test, but in a static-CRT (/MT) DLL. That DLL cannot be VLD-instrumented
// (use_vld pulls the /MD CRT and conflicts, LNK4098), so its allocations are invisible to the leak
// check and the leak goes undetected: this process exits 0. The test documents that limitation.

#include "windows.h"

#include "ctest.h"

typedef void (*leak_check_helper_leak_fn)(void);

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_dll_mt_real_leak_int)

CTEST_FUNCTION(leak_inside_static_crt_dll_is_not_detected)
{
    HMODULE module = LoadLibraryA("ctest_leak_check_helper_dll_mt.dll");
    CTEST_ASSERT_IS_TRUE(module != NULL);

    leak_check_helper_leak_fn leak = (leak_check_helper_leak_fn)GetProcAddress(module, "leak_check_helper_leak");
    CTEST_ASSERT_IS_TRUE(leak != NULL);
    leak();
}

CTEST_END_TEST_SUITE(ctest_leak_check_dll_mt_real_leak_int)
