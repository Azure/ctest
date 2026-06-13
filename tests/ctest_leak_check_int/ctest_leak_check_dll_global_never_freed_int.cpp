// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// The helper DLL is loaded with LoadLibrary during the test (its global is constructed after the
// leak baseline) and is deliberately never freed. Its destructor still runs during process teardown
// before the at-exit check samples leaks, so the well-behaved global is not reported. This process
// exits 0.

#include "windows.h"

#include "ctest.h"

typedef int (*leak_check_helper_touch_fn)(void);

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_dll_global_never_freed_int)

CTEST_FUNCTION(loadlibrary_dll_global_never_freed)
{
    HMODULE module = LoadLibraryA("ctest_leak_check_helper_dll.dll");
    CTEST_ASSERT_IS_TRUE(module != NULL);

    leak_check_helper_touch_fn touch = (leak_check_helper_touch_fn)GetProcAddress(module, "leak_check_helper_touch");
    CTEST_ASSERT_IS_TRUE(touch != NULL);
    CTEST_ASSERT_IS_TRUE(touch() > 0);

    // Intentionally no FreeLibrary.
}

CTEST_END_TEST_SUITE(ctest_leak_check_dll_global_never_freed_int)
