// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// C runner for the C++ suite: CTEST_RUN_TEST_SUITE uses C linkage at global scope, so it cannot
// live inside a C++ function body.

#include <stddef.h>

#include "c_logging/logger.h"

#include "ctest.h"

int main(void)
{
    size_t failedTestCount = 0;

    (void)logger_init();

    CTEST_RUN_TEST_SUITE(ctest_leak_check_process_lifetime_static_int, failedTestCount);

    logger_deinit();

    return (int)failedTestCount;
}
