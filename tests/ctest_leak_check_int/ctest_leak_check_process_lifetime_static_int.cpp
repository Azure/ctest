// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Regression test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// A function-local process-lifetime static (the azure-core Url::Encode pattern) is constructed
// during a test and freed only at process exit. The old end-of-RunTests sampling misreported it as
// a leak (exit -8 on the ARM64 Debug gate); the at-exit check must not, so this process exits 0.

#include <cctype>
#include <unordered_set>

#include "ctest.h"

namespace {
bool should_encode(char c)
{
    static std::unordered_set<char> const extra_non_encodable_chars = {'-', '.', '_', '~'};

    return !std::isalnum(static_cast<unsigned char>(c))
        && extra_non_encodable_chars.find(c) == extra_non_encodable_chars.end();
}
} // namespace

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_process_lifetime_static_int)

CTEST_FUNCTION(process_lifetime_static_is_not_reported_as_leak)
{
    CTEST_ASSERT_IS_TRUE(should_encode(' '));
    CTEST_ASSERT_IS_TRUE(!should_encode('-'));
}

CTEST_END_TEST_SUITE(ctest_leak_check_process_lifetime_static_int)
