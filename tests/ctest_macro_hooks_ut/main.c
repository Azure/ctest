// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stddef.h>  // for size_t

#include "c_logging/logger.h"

#include "ctest.h"

int main()
{
    size_t failedTests = 0;

    (void)logger_init();

    CTEST_RUN_TEST_SUITE(ctest_macro_hooks_ut);

    logger_deinit();

    return failedTests;
}
