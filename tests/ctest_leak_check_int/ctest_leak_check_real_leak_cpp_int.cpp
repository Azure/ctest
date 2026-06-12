// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// A C++ allocation (operator new) that is never freed is a genuine leak surviving teardown, so the
// at-exit check must force a non-zero process exit. The test is registered WILL_FAIL.

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_real_leak_cpp_int)

CTEST_FUNCTION(cpp_allocation_that_is_never_freed_is_reported_as_leak)
{
    int* block = new int[16];
    CTEST_ASSERT_IS_NOT_NULL(block);
    // Intentionally never deleted: a genuine C++ leak that survives teardown and must be detected.
}

CTEST_END_TEST_SUITE(ctest_leak_check_real_leak_cpp_int)
