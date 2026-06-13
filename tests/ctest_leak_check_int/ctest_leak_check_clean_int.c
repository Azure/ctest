// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Integration test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// An allocation freed before the test ends is not a leak, so this process exits 0.

#include <stddef.h>
#include <stdlib.h>

#include "c_logging/logger.h"

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_clean_int)

CTEST_FUNCTION(allocation_that_is_freed_is_not_reported_as_leak)
{
    void* block = malloc(64);
    CTEST_ASSERT_IS_NOT_NULL(block);
    free(block);
}

CTEST_END_TEST_SUITE(ctest_leak_check_clean_int)

int main(void)
{
    size_t failedTestCount = 0;

    (void)logger_init();

    CTEST_RUN_TEST_SUITE(ctest_leak_check_clean_int, failedTestCount);

    logger_deinit();

    return (int)failedTestCount;
}
