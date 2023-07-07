// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef CTEST_TEST_HOOKS_H
#define CTEST_TEST_HOOKS_H

#include "macro_utils/macro_utils.h"

// For gcc build that causes a false negative
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#define CTEST_CUSTOM_TEST_SUITE_INITIALIZE_CODE(suiteName) \
    static int MU_C2(test_variable_suite_initialize_, suiteName) = 42;

#define CTEST_CUSTOM_TEST_SUITE_CLEANUP_CODE(suiteName) \
    static int MU_C2(test_variable_suite_cleanup_, suiteName) = 43;

#define CTEST_CUSTOM_TEST_FUNCTION_INITIALIZE_CODE(funcName) \
    static int MU_C2(test_variable_function_initialize_, funcName) = 44;

#define CTEST_CUSTOM_TEST_FUNCTION_CLEANUP_CODE(funcName) \
    static int MU_C2(test_variable_function_cleanup_, funcName) = 45;

#define CTEST_CUSTOM_TEST_FUNCTION_CODE(funcName) \
    static int MU_C2(test_variable_function_, funcName) = 46;

#endif /* CTEST_TEST_HOOKS_H */
