// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "ctest.h"

/* Static variables to track which tests were executed */
static int g_FilterTestSuiteTest1_was_executed = 0;
static int g_FilterTestSuiteTest2_was_executed = 0;
static int g_FilterTestSuiteTest3_was_executed = 0;

/* Function to reset execution tracking */
void FilterTestSuite_ResetExecutionTracking(void)
{
    g_FilterTestSuiteTest1_was_executed = 0;
    g_FilterTestSuiteTest2_was_executed = 0;
    g_FilterTestSuiteTest3_was_executed = 0;
}

/* Function to check which tests were executed */
int FilterTestSuite_WasTest1Executed(void)
{
    return g_FilterTestSuiteTest1_was_executed;
}

int FilterTestSuite_WasTest2Executed(void)
{
    return g_FilterTestSuiteTest2_was_executed;
}

int FilterTestSuite_WasTest3Executed(void)
{
    return g_FilterTestSuiteTest3_was_executed;
}

CTEST_BEGIN_TEST_SUITE(FilterTestSuite)

CTEST_FUNCTION(FilterTest1)
{
    g_FilterTestSuiteTest1_was_executed = 1;
}

CTEST_FUNCTION(FilterTest2)
{
    g_FilterTestSuiteTest2_was_executed = 1;
}

CTEST_FUNCTION(FilterTest3)
{
    g_FilterTestSuiteTest3_was_executed = 1;
}

CTEST_END_TEST_SUITE(FilterTestSuite)
