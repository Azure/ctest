// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Regression test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
//
// A function-local, process-lifetime static -- the exact pattern the Azure SDK for C++ uses in
// azure-core's Url::Encode (a lazily-initialized std::unordered_set<char> of characters that must
// not be percent-encoded) -- is constructed on first use during a test and is still alive at the
// end of RunTests, but is freed by its destructor at process exit. The previous end-of-RunTests
// leak sampling counted it as a leak (e.g. exit code -8 on the ARM64 Debug gate); the at-exit check
// must NOT, so this process must exit 0.

#include <cctype>
#include <unordered_set>

#include "ctest.h"

namespace {
// Mirrors azure-core-cpp's ShouldEncode lookup set: a lazily-initialized, process-lifetime static.
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
    // First use constructs the static during the test (after the leak baseline). It lives until
    // process exit and is freed by its destructor, so the at-exit check must not flag it.
    CTEST_ASSERT_IS_TRUE(should_encode(' '));  // a space must be encoded
    CTEST_ASSERT_IS_TRUE(!should_encode('-')); // '-' is in the non-encodable set
}

CTEST_END_TEST_SUITE(ctest_leak_check_process_lifetime_static_int)
