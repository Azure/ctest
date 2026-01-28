// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>  // for printf

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_one_custom_fixture_ut)

static int times_suite_initialize_called = 0;
static int times_suite_cleanup_called = 0;
static int times_function_initialize_called = 0;
static int times_function_cleanup_called = 0;
static int expected_times_function_initialize_called = 1;
static int expected_times_function_cleanup_called = 0;

static void suite_initialize_custom_fixture(void)
{
    times_suite_initialize_called++;
}

static void suite_cleanup_custom_fixture(void)
{
    times_suite_cleanup_called++;
}

static void function_initialize_custom_fixture(void)
{
    times_function_initialize_called++;
}

static void function_cleanup_custom_fixture(void)
{
    times_function_cleanup_called++;
}

CTEST_SUITE_INITIALIZE(a, suite_initialize_custom_fixture)
{
    // make sure that custom fixtures have already been called
    CTEST_ASSERT_ARE_EQUAL(int, 1, times_suite_initialize_called);
}

CTEST_SUITE_CLEANUP(b, suite_cleanup_custom_fixture)
{
    // make sure that the cleanup fixture was not yet called
    CTEST_ASSERT_ARE_EQUAL(int, 0, times_suite_cleanup_called);
}

CTEST_FUNCTION_INITIALIZE(c, function_initialize_custom_fixture)
{
    // check that the function initialize custom fixture was already called by the time we got here
    CTEST_ASSERT_ARE_EQUAL(int, expected_times_function_initialize_called, times_function_initialize_called);
    expected_times_function_initialize_called++;
}

CTEST_FUNCTION_CLEANUP(d, function_cleanup_custom_fixture)
{
    // check that the function initialize custom fixture was not yet called by the time we got here
    CTEST_ASSERT_ARE_EQUAL(int, expected_times_function_cleanup_called, times_function_cleanup_called);
    expected_times_function_cleanup_called++;
}

CTEST_FUNCTION(ctest_function_initialize_and_cleanuip_custom_fixtures_gets_executed)
{
    // arrange

    // act - done implicitly by the test framework calling the fixture hooks

    // assert
    // 2 test functions have been executed at this point, so the variable was incremented twice
    CTEST_ASSERT_ARE_EQUAL(int, 2, times_function_initialize_called);
    // 1 cleanup has been executed by this time
    CTEST_ASSERT_ARE_EQUAL(int, 1, times_function_cleanup_called);
}

CTEST_FUNCTION(ctest_function_cleanup_custom_fixture_gets_executed)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    // 1 test functions have been executed at this point, so the variable was incremented twice 
    CTEST_ASSERT_ARE_EQUAL(int, 1, times_function_initialize_called);
    // 0 cleanup has been executed by this time
    CTEST_ASSERT_ARE_EQUAL(int, 0, times_function_cleanup_called);
}

CTEST_END_TEST_SUITE(ctest_one_custom_fixture_ut)

// this function exists so that we can check from the main whether teh final suite cleanup call really happened
int ctest_one_custom_fixture_test_succeeded(void)
{
    int result;

    if (times_suite_cleanup_called != 1)
    {
        (void)printf("times_suite_cleanup_called should be 1\r\n");
        result = MU_FAILURE;
    }
    else
    {
        result = 0;
    }

    return result;
}
