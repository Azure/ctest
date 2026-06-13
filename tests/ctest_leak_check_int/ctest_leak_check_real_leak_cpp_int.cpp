// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// A C++ allocation (operator new) that is never freed survives teardown, so the at-exit check
// forces a non-zero exit. Registered WILL_FAIL.

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_real_leak_cpp_int)

CTEST_FUNCTION(cpp_allocation_that_is_never_freed_is_reported_as_leak)
{
    int* block = new int[16];
    CTEST_ASSERT_IS_NOT_NULL(block);
}

CTEST_END_TEST_SUITE(ctest_leak_check_real_leak_cpp_int)
