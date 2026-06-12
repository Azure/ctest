// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// A genuine leak made inside a LoadLibrary'd DLL (default /MD CRT) must be detected, proving the
// leak detector tracks allocations originating in a dynamically loaded module. Registered WILL_FAIL.

#include "windows.h"

#include "ctest.h"

typedef void (*leak_check_helper_leak_fn)(void);

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_dll_real_leak_int)

CTEST_FUNCTION(leak_inside_loaded_dll_is_reported_as_leak)
{
    HMODULE module = LoadLibraryA("ctest_leak_check_helper_dll.dll");
    CTEST_ASSERT_IS_TRUE(module != NULL);

    leak_check_helper_leak_fn leak = (leak_check_helper_leak_fn)GetProcAddress(module, "leak_check_helper_leak");
    CTEST_ASSERT_IS_TRUE(leak != NULL);
    leak();
}

CTEST_END_TEST_SUITE(ctest_leak_check_dll_real_leak_int)
