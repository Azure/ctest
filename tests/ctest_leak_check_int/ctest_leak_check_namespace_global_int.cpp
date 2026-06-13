// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Regression test for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
// A C++ namespace-scope global is constructed before main, so its allocation is captured in the
// leak baseline; the at-exit check must not report it. This process must exit 0.

#include "ctest.h"

namespace {
const int global_allocation_size = 128;

class lifetime_buffer
{
public:
    lifetime_buffer() : data_(new char[global_allocation_size]) {}
    ~lifetime_buffer() { delete[] data_; }
    int size() const { return global_allocation_size; }
private:
    char* data_;
};

lifetime_buffer g_global_buffer;
} // namespace

CTEST_BEGIN_TEST_SUITE(ctest_leak_check_namespace_global_int)

CTEST_FUNCTION(namespace_global_allocation_is_not_reported_as_leak)
{
    CTEST_ASSERT_IS_TRUE(g_global_buffer.size() == global_allocation_size);
}

CTEST_END_TEST_SUITE(ctest_leak_check_namespace_global_int)
