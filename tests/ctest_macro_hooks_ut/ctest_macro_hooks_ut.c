// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "ctest_test_hooks.h"
#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(ctest_macro_hooks_ut)

CTEST_SUITE_INITIALIZE(a)
{
    // nothing to do here
}

CTEST_SUITE_CLEANUP(b)
{
    // nothing to do here
}

CTEST_FUNCTION_INITIALIZE(c)
{
    // nothing to do here
}

CTEST_FUNCTION_CLEANUP(d)
{
    // nothing to do here
}

CTEST_FUNCTION(ctest_suite_initialize_macro_hook_generates_a_variable)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, 42, MU_C2(test_variable_suite_initialize_, a));
}

CTEST_FUNCTION(ctest_suite_cleanup_macro_hook_generates_a_variable)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, 43, MU_C2(test_variable_suite_cleanup_, b));
}

CTEST_FUNCTION(ctest_function_initialize_macro_hook_generates_a_variable)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, 44, MU_C2(test_variable_function_initialize_, c));
}

CTEST_FUNCTION(ctest_function_cleanup_macro_hook_generates_a_variable)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, 45, MU_C2(test_variable_function_cleanup_, d));
}

CTEST_FUNCTION(ctest_function_macro_hook_generates_a_variable)
{
    // arrange
    // act is done by the virtue of using the hooks

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, 46, MU_C2(test_variable_function_, ctest_function_macro_hook_generates_a_variable));
}

CTEST_END_TEST_SUITE(ctest_macro_hooks_ut)
