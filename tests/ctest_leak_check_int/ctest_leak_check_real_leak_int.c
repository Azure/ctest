// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// An allocation that is never freed survives all teardown and is a genuine leak, so the at-exit
// check must force a non-zero process exit. The test is registered WILL_FAIL.

#include <stddef.h>
#include <stdlib.h>

#include "c_logging/logger.h"

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_real_leak_int)

CTEST_FUNCTION(allocation_that_is_never_freed_is_reported_as_leak)
{
    void* block = malloc(64);
    CTEST_ASSERT_IS_NOT_NULL(block);
    // Intentionally never freed: a genuine leak that survives teardown and must be detected.
}

CTEST_END_TEST_SUITE(ctest_leak_check_real_leak_int)

int main(void)
{
    size_t failedTestCount = 0;

    (void)logger_init();

    CTEST_RUN_TEST_SUITE(ctest_leak_check_real_leak_int, failedTestCount);

    logger_deinit();

    return (int)failedTestCount;
}
