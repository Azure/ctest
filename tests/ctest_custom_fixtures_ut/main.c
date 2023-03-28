// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>  // for size_t
#include "ctest.h"

#include "ctest_one_custom_fixture_ut.h"
#include "ctest_multiple_custom_fixtures_ut.h"

int main()
{
    size_t failedTests = 0;
    CTEST_RUN_TEST_SUITE(ctest_one_custom_fixture_ut);
    if (ctest_one_custom_fixture_test_succeeded() != 0)
    {
        // if the final validation of how we called the fixtures has failed it means at least one test is failed
        failedTests++;
    }
    CTEST_RUN_TEST_SUITE(ctest_multiple_custom_fixtures_ut);
    if (ctest_multiple_custom_fixtures_test_succeeded() != 0)
    {
        // if the final validation of how we called the fixtures has failed it means at least one test is failed
        failedTests++;
    }
    return failedTests;
}
