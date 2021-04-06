// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>  // for size_t
#include "ctest.h"

int main()
{
    size_t failedTests = 0;
    CTEST_RUN_TEST_SUITE(ctest_macro_hooks_ut);
    return failedTests;
}
