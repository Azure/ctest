// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>  // for printf

#include "macro_utils/macro_utils.h" // IWYU pragma: keep

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_multiple_custom_fixtures_ut)

#define FIXTURE_CALL_VALUES \
    FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, \
    FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2, \
    FIXTURE_CALL_USER_SUITE_INITIALIZE, \
    FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_1, \
    FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_2, \
    FIXTURE_CALL_USER_SUITE_CLEANUP, \
    FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, \
    FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2, \
    FIXTURE_CALL_USER_FUNCTION_INITIALIZE, \
    FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, \
    FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2, \
    FIXTURE_CALL_USER_FUNCTION_CLEANUP

MU_DEFINE_ENUM(FIXTURE_CALL_TYPE, FIXTURE_CALL_VALUES)

MU_DEFINE_ENUM_STRINGS(FIXTURE_CALL_TYPE, FIXTURE_CALL_VALUES)
CTEST_DEFINE_ENUM_TYPE(FIXTURE_CALL_TYPE, FIXTURE_CALL_VALUES)

static FIXTURE_CALL_TYPE actual_calls[100];
static int actual_call_count = 0;

static void add_call(FIXTURE_CALL_TYPE fixture_call_type)
{
    actual_calls[actual_call_count++] = fixture_call_type;
}

static void verify_expected_calls(FIXTURE_CALL_TYPE* expected_calls, int expected_call_count)
{
    CTEST_ASSERT_ARE_EQUAL(int, expected_call_count, actual_call_count);

    for (int i = 0; i < expected_call_count; i++)
    {
        CTEST_ASSERT_ARE_EQUAL(FIXTURE_CALL_TYPE, expected_calls[i], actual_calls[i]);
    }
}

static void suite_initialize_custom_fixture_1(void)
{
    add_call(FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1);
}

static void suite_initialize_custom_fixture_2(void)
{
    add_call(FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2);
}

static void suite_cleanup_custom_fixture_1(void)
{
    add_call(FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_1);
}

static void suite_cleanup_custom_fixture_2(void)
{
    add_call(FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_2);
}

static void function_initialize_custom_fixture_1(void)
{
    add_call(FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1);
}

static void function_initialize_custom_fixture_2(void)
{
    add_call(FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2);
}

static void function_cleanup_custom_fixture_1(void)
{
    add_call(FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1);
}

static void function_cleanup_custom_fixture_2(void)
{
    add_call(FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2);
}

CTEST_SUITE_INITIALIZE(a, suite_initialize_custom_fixture_1, suite_initialize_custom_fixture_2)
{
    FIXTURE_CALL_TYPE expected_calls[] = { FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2 };
    verify_expected_calls(expected_calls, MU_COUNT_ARRAY_ITEMS(expected_calls));
    add_call(FIXTURE_CALL_USER_SUITE_INITIALIZE);
}

CTEST_SUITE_CLEANUP(b, suite_cleanup_custom_fixture_1, suite_cleanup_custom_fixture_2)
{
    FIXTURE_CALL_TYPE expected_calls[] = { 
        FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2,
        FIXTURE_CALL_USER_SUITE_INITIALIZE,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
        FIXTURE_CALL_USER_FUNCTION_CLEANUP,
        FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
        FIXTURE_CALL_USER_FUNCTION_CLEANUP,
        FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2,
    };
    verify_expected_calls(expected_calls, MU_COUNT_ARRAY_ITEMS(expected_calls));
    add_call(FIXTURE_CALL_USER_SUITE_CLEANUP);
}

CTEST_FUNCTION_INITIALIZE(c, function_initialize_custom_fixture_1, function_initialize_custom_fixture_2)
{
    add_call(FIXTURE_CALL_USER_FUNCTION_INITIALIZE);
}

CTEST_FUNCTION_CLEANUP(d, function_cleanup_custom_fixture_1, function_cleanup_custom_fixture_2)
{
    add_call(FIXTURE_CALL_USER_FUNCTION_CLEANUP);
}

CTEST_FUNCTION(ctest_function_initialize_and_cleanuip_custom_fixtures_gets_executed)
{
    // arrange

    // assert
    FIXTURE_CALL_TYPE expected_calls[] = {
    FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2,
        FIXTURE_CALL_USER_SUITE_INITIALIZE,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
        FIXTURE_CALL_USER_FUNCTION_CLEANUP,
        FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
    };
    verify_expected_calls(expected_calls, MU_COUNT_ARRAY_ITEMS(expected_calls));
}

CTEST_FUNCTION(ctest_function_cleanup_custom_fixture_gets_executed)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    FIXTURE_CALL_TYPE expected_calls[] = { 
    FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2,
        FIXTURE_CALL_USER_SUITE_INITIALIZE,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
    };
    verify_expected_calls(expected_calls, MU_COUNT_ARRAY_ITEMS(expected_calls));
}

CTEST_END_TEST_SUITE(ctest_multiple_custom_fixtures_ut)

// this function exists so that we can check from the main whether all calls really happened 
// and they happened in the order that we wanted
int ctest_multiple_custom_fixtures_test_succeeded(void)
{
    int result;

    FIXTURE_CALL_TYPE expected_calls[] = {
        FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_1, FIXTURE_CALL_CUSTOM_SUITE_INITIALIZE_2,
        FIXTURE_CALL_USER_SUITE_INITIALIZE,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
        FIXTURE_CALL_USER_FUNCTION_CLEANUP,
        FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2,
        FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_1, FIXTURE_CALL_CUSTOM_FUNCTION_INITIALIZE_2,
        FIXTURE_CALL_USER_FUNCTION_INITIALIZE,
        FIXTURE_CALL_USER_FUNCTION_CLEANUP,
        FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_1, FIXTURE_CALL_CUSTOM_FUNCTION_CLEANUP_2,
        FIXTURE_CALL_USER_SUITE_CLEANUP,
        FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_1, FIXTURE_CALL_CUSTOM_SUITE_CLEANUP_2,
    };

    int expected_call_count = MU_COUNT_ARRAY_ITEMS(expected_calls);
    if (expected_call_count != actual_call_count)
    {
        (void)printf("expected_call_count=%d, actual_call_count=%d\r\n", expected_call_count, actual_call_count);
        result = MU_FAILURE;
    }
    else
    {
        int i;

        for (i = 0; i < expected_call_count; i++)
        {
            if (expected_calls[i] != actual_calls[i])
            {
                (void)printf("expected_calls[i]=%" PRI_MU_ENUM ", actual_calls[i]=%" PRI_MU_ENUM "\r\n", MU_ENUM_VALUE(FIXTURE_CALL_TYPE, expected_calls[i]), MU_ENUM_VALUE(FIXTURE_CALL_TYPE, actual_calls[i]));
                break;
            }
        }

        if (i < expected_call_count)
        {
            result = MU_FAILURE;
        }
        else
        {
            result = 0;
        }
    }

    return result;
}
