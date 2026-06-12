// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Regression test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// The helper DLL is load-time (implicitly) linked, so its process-lifetime global is constructed
// during DLL initialization before main and is captured in the leak baseline. This process exits 0.

#include "ctest.h"

#include "leak_check_helper.h"

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_dll_global_loadtime_int)

CTEST_FUNCTION(loadtime_dll_global_allocation_is_not_reported_as_leak)
{
    CTEST_ASSERT_IS_TRUE(leak_check_helper_touch() > 0);
}

CTEST_END_TEST_SUITE(ctest_leak_check_dll_global_loadtime_int)
