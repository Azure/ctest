// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>  // for size_t

#include "c_logging/logger.h"

#include "ctest.h"

#include "testnamefiltertests.h"

int main()
{
    size_t failedTests = 0;

    (void)logger_init();

    /* This first suite is ran without counting failed tests to prove that the argument is optional. */
    CTEST_RUN_TEST_SUITE(SimpleTestSuiteOneTest);

    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(enum_define_tests, temp_failed_tests);
        if (temp_failed_tests != 2) // 2 expected failing tests
        {
            LogError("CTEST TEST FAILED !!! enum_define_tests");
            failedTests++;
        }
    }
    CTEST_RUN_TEST_SUITE(SimpleTestSuiteOneTest, failedTests);
    CTEST_RUN_TEST_SUITE(SimpleTestSuiteTwoTests, failedTests);
    CTEST_RUN_TEST_SUITE(SimpleTestSuiteOneTestCpp, failedTests);
    CTEST_RUN_TEST_SUITE(TestSuiteInitializeCleanupTests, failedTests);
    CTEST_RUN_TEST_SUITE(AssertSuccessTests, failedTests);
    CTEST_RUN_TEST_SUITE(AssertSuccessTestsCpp, failedTests);
    {
        // tests with regular assert failures, no special messages
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(AssertFailureTests, temp_failed_tests);
        if (temp_failed_tests != 75)
        {
            LogError("CTEST TEST FAILED !!! AssertFailureTests");
            failedTests ++;
        }
    }
    {
        // tests with assert failures with printf like messages
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(AssertFailureTestsWithPrintfLikeMsgs, temp_failed_tests);
        if (temp_failed_tests != 38)
        {
            LogError("CTEST TEST FAILED !!! AssertFailureTestsWithPrintfLikeMsgs");
            failedTests++;
        }
    }
    CTEST_RUN_TEST_SUITE(TestFunctionInitializeTests, failedTests);
    CTEST_RUN_TEST_SUITE(TestFunctionCleanupTests, failedTests);

    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(whentestfunctioninitializefailstests, temp_failed_tests);
        if (temp_failed_tests != 2)
        {
            LogError("CTEST TEST FAILED !!! whentestfunctioninitializefailstests");
            failedTests ++;
        }
    }
    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(whentestsuiteinitializefailstests, temp_failed_tests);
        if (temp_failed_tests != 1)
        {
            LogError("CTEST TEST FAILED !!! whentestsuiteinitializefailstests");
            failedTests ++;
        }
    }

    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(testfunctioncleanupfailstests, temp_failed_tests);
        if (temp_failed_tests != 2)
        {
            LogError("CTEST TEST FAILED !!! testfunctioncleanupfailstests");
            failedTests ++;
        }
    }

    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(TestSuiteCleanupTests, temp_failed_tests);
        if (temp_failed_tests == 0) /*the tests in TestSuiteCleanupTests HAVE to fail*/
        {
            LogError("CTEST TEST FAILED !!! TestSuiteCleanupTests");
            failedTests++;
        }
    }

    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(TestSuiteCleanupTests2, temp_failed_tests);
        if (temp_failed_tests == 0) /*the tests in TestSuiteCleanupTests HAVE to fail*/
        {
            LogError("CTEST TEST FAILED !!! TestSuiteCleanupTests2");
            failedTests++;
        }
    }

#if defined _MSC_VER
    {
        size_t temp_failed_tests = 0;
        CTEST_RUN_TEST_SUITE(WindowsTypesTests, temp_failed_tests);
        if (temp_failed_tests != 5) // 5 expected failing tests
        {
            LogError("CTEST TEST FAILED !!! WindowsTypesTests");
            failedTests++;
        }
    }
#endif

    /* Test name filter tests */
    {
        /* Test: Run all tests without a filter (NULL) */
        size_t temp_failed_tests = 0;
        FilterTestSuite_ResetExecutionTracking();
        CTEST_RUN_TEST_SUITE(FilterTestSuite, temp_failed_tests);
        if (temp_failed_tests != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite without filter");
            failedTests++;
        }
        if (FilterTestSuite_WasTest1Executed() != 1 ||
            FilterTestSuite_WasTest2Executed() != 1 ||
            FilterTestSuite_WasTest3Executed() != 1)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite without filter did not run all tests");
            failedTests++;
        }
    }

    {
        /* Test: Run only FilterTest2 using the filter */
        size_t temp_failed_tests = 0;
        FilterTestSuite_ResetExecutionTracking();
        CTEST_RUN_TEST_SUITE(FilterTestSuite, temp_failed_tests, "FilterTest2");
        if (temp_failed_tests != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with filter failed");
            failedTests++;
        }
        if (FilterTestSuite_WasTest1Executed() != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with filter: FilterTest1 should NOT have been executed");
            failedTests++;
        }
        if (FilterTestSuite_WasTest2Executed() != 1)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with filter: FilterTest2 should have been executed");
            failedTests++;
        }
        if (FilterTestSuite_WasTest3Executed() != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with filter: FilterTest3 should NOT have been executed");
            failedTests++;
        }
    }

    {
        /* Test: Run with empty string filter (should run all tests) */
        size_t temp_failed_tests = 0;
        FilterTestSuite_ResetExecutionTracking();
        CTEST_RUN_TEST_SUITE(FilterTestSuite, temp_failed_tests, "");
        if (temp_failed_tests != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with empty filter");
            failedTests++;
        }
        if (FilterTestSuite_WasTest1Executed() != 1 ||
            FilterTestSuite_WasTest2Executed() != 1 ||
            FilterTestSuite_WasTest3Executed() != 1)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with empty filter did not run all tests");
            failedTests++;
        }
    }

    {
        /* Test: Run with filter for non-existent test (should run no tests) */
        size_t temp_failed_tests = 0;
        FilterTestSuite_ResetExecutionTracking();
        CTEST_RUN_TEST_SUITE(FilterTestSuite, temp_failed_tests, "NonExistentTest");
        if (temp_failed_tests != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with non-existent filter");
            failedTests++;
        }
        if (FilterTestSuite_WasTest1Executed() != 0 ||
            FilterTestSuite_WasTest2Executed() != 0 ||
            FilterTestSuite_WasTest3Executed() != 0)
        {
            LogError("CTEST TEST FAILED !!! FilterTestSuite with non-existent filter should not run any tests");
            failedTests++;
        }
    }

    logger_deinit();

    return failedTests;
}
