// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>  // for size_t

#include "c_logging/logger.h"

#include "ctest.h"

#include "ctest_parameterized_ut.h"

int main(void)
{
    size_t failedTests = 0;

    (void)logger_init();

    ctest_parameterized_ut_reset_execution_tracking();
    CTEST_RUN_TEST_SUITE(ctest_parameterized_ut, failedTests);

    /* Verify that the parameterized test cases for test_verify_calls were all executed */
    /* 4 cases: (1,10), (2,20), (3,30), (4,40) => call_count=4, accumulated_sum = 11+22+33+44 = 110 */
    if (ctest_parameterized_ut_get_call_count() != 4)
    {
        LogError("CTEST TEST FAILED !!! Expected 4 parameterized test cases to execute, but got %d", ctest_parameterized_ut_get_call_count());
        failedTests++;
    }
    if (ctest_parameterized_ut_get_accumulated_sum() != 110)
    {
        LogError("CTEST TEST FAILED !!! Expected accumulated sum of 110, but got %d", ctest_parameterized_ut_get_accumulated_sum());
        failedTests++;
    }

    logger_deinit();

    return (int)failedTests;
}
