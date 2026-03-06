// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "ctest.h"

#include "ctest_parameterized_ut.h"

static int add_numbers(int a, int b)
{
    return a + b;
}

static int multiply_numbers(int a, int b)
{
    return a * b;
}

/* Global state to verify parameterized test cases are actually executed with correct parameters */
static int g_call_count;
static int g_accumulated_sum;

void ctest_parameterized_ut_reset_execution_tracking(void)
{
    g_call_count = 0;
    g_accumulated_sum = 0;
}

int ctest_parameterized_ut_get_call_count(void)
{
    return g_call_count;
}

int ctest_parameterized_ut_get_accumulated_sum(void)
{
    return g_accumulated_sum;
}

CTEST_BEGIN_TEST_SUITE(ctest_parameterized_ut)

CTEST_SUITE_INITIALIZE(suite_init)
{
}

CTEST_SUITE_CLEANUP(suite_cleanup)
{
}

CTEST_FUNCTION_INITIALIZE(test_init)
{
}

CTEST_FUNCTION_CLEANUP(test_cleanup)
{
}

/*
 * Verification test: records global state to prove each CASE is executed with correct parameters.
 * main.c validates that all 4 cases ran and produced the expected accumulated sum.
 */
CTEST_PARAMETERIZED_TEST_FUNCTION(test_verify_calls,
    ARGS(int, a, int, b),
    CASE((1, 10), case_1_10),
    CASE((2, 20), case_2_20),
    CASE((3, 30), case_3_30),
    CASE((4, 40), case_4_40))
{
    // arrange
    // (parameters a and b are set by the CASE macro)

    // act
    g_call_count++;
    g_accumulated_sum += a + b;

    // assert
    CTEST_ASSERT_IS_TRUE(a >= 1, "Expected a >= 1, got %d", a);
    CTEST_ASSERT_IS_TRUE(b >= 10, "Expected b >= 10, got %d", b);
}

CTEST_PARAMETERIZED_TEST_FUNCTION(test_addition,
    ARGS(int, a, int, b, int, expected),
    CASE((0, 0, 0), with_zeros),
    CASE((1, 2, 3), with_small_positive_numbers),
    CASE((-1, 1, 0), with_negative_and_positive),
    CASE((100, 200, 300), with_larger_numbers))
{
    // arrange
    // (parameters a, b, expected are set by the CASE macro)

    // act
    int result = add_numbers(a, b);

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, expected, result, "Expected %d + %d = %d, but got %d", a, b, expected, result);
}

CTEST_PARAMETERIZED_TEST_FUNCTION(test_multiplication,
    ARGS(int, x, int, y, int, expected_product),
    CASE((0, 5, 0), with_zero),
    CASE((1, 1, 1), with_ones),
    CASE((2, 3, 6), with_small_numbers),
    CASE((-2, 3, -6), with_negative_factor))
{
    // arrange
    // (parameters x, y, expected_product are set by the CASE macro)

    // act
    int result = multiply_numbers(x, y);

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, expected_product, result);
}

CTEST_PARAMETERIZED_TEST_FUNCTION(test_is_positive,
    ARGS(int, value, int, expected_is_positive),
    CASE((1, 1), for_positive_one),
    CASE((100, 1), for_hundred),
    CASE((0, 0), for_zero),
    CASE((-1, 0), for_negative_one))
{
    // arrange
    // (parameters value, expected_is_positive are set by the CASE macro)

    // act
    int result = (value > 0) ? 1 : 0;

    // assert
    CTEST_ASSERT_ARE_EQUAL(int, expected_is_positive, result);
}

CTEST_END_TEST_SUITE(ctest_parameterized_ut)
