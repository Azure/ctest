// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// The same helper is present twice: statically linked into this executable (its global is
// constructed before main, inside the leak baseline) and as a separately loaded DLL built with the
// static CRT (a different heap from this /MD executable). The DLL is loaded and freed during the
// test. Neither the duplicated global nor the cross-CRT allocation must produce a spurious leak, so
// this process exits 0.

#include "windows.h"

#include "ctest.h"

#include "leak_check_helper.h"

typedef int (*leak_check_helper_touch_fn)(void);

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_dll_dual_link_int)

CTEST_FUNCTION(static_and_dynamic_linkage_of_same_helper_is_not_reported_as_leak)
{
    // Statically linked copy: resolved directly by symbol.
    CTEST_ASSERT_IS_TRUE(leak_check_helper_touch() > 0);

    // Dynamically loaded copy with the static CRT: resolved via GetProcAddress, then freed.
    HMODULE module = LoadLibraryA("ctest_leak_check_helper_dll_mt.dll");
    CTEST_ASSERT_IS_TRUE(module != NULL);

    leak_check_helper_touch_fn touch = (leak_check_helper_touch_fn)GetProcAddress(module, "leak_check_helper_touch");
    CTEST_ASSERT_IS_TRUE(touch != NULL);
    CTEST_ASSERT_IS_TRUE(touch() > 0);

    CTEST_ASSERT_IS_TRUE(FreeLibrary(module) != FALSE);
}

CTEST_END_TEST_SUITE(ctest_leak_check_dll_dual_link_int)
