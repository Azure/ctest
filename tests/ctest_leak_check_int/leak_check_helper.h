// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Shared helper for the leak-check integration tests. Built three ways (see CMakeLists.txt): a
// shared DLL, a static library, and a shared DLL with the static CRT. Each module instance owns one
// process-lifetime global that allocates on construction and frees on destruction.

#ifndef LEAK_CHECK_HELPER_H
#define LEAK_CHECK_HELPER_H

#if defined(LEAK_CHECK_HELPER_STATIC)
#define LEAK_CHECK_HELPER_API
#elif defined(LEAK_CHECK_HELPER_EXPORTS)
#define LEAK_CHECK_HELPER_API __declspec(dllexport)
#else
#define LEAK_CHECK_HELPER_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Returns the size of this module's process-lifetime global allocation. Referencing it forces the
// module to load (and its global to be constructed) and confirms the global is live.
LEAK_CHECK_HELPER_API int leak_check_helper_touch(void);

// Allocates a block inside this module and never frees it: a genuine leak originating in this
// module. Used to confirm the leak detector actually tracks allocations made by this module.
LEAK_CHECK_HELPER_API void leak_check_helper_leak(void);

#ifdef __cplusplus
}
#endif

#endif // LEAK_CHECK_HELPER_H
