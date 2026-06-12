// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "leak_check_helper.h"

namespace {
const int helper_allocation_size = 256;

// Process-lifetime global: allocates on construction (module load / pre-main static init) and frees
// on destruction (module unload / process exit). This mirrors a well-behaved SDK global that is not
// a leak, but whose teardown timing relative to the test runner's at-exit check is what the tests
// exercise.
class lifetime_buffer
{
public:
    lifetime_buffer() : data_(new char[helper_allocation_size]) {}
    ~lifetime_buffer() { delete[] data_; }
    int size() const { return helper_allocation_size; }
private:
    char* data_;
};

lifetime_buffer g_lifetime_buffer;

// Keeps the leaked pointer reachable so the compiler cannot elide the allocation.
void* volatile g_leak_sink;
} // namespace

extern "C" LEAK_CHECK_HELPER_API int leak_check_helper_touch(void)
{
    return g_lifetime_buffer.size();
}

extern "C" LEAK_CHECK_HELPER_API void leak_check_helper_leak(void)
{
    g_leak_sink = new char[helper_allocation_size];
}
