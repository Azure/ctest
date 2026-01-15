// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>
#include <stdbool.h>
#include <setjmp.h>
#include <stddef.h>
#include <wchar.h>
#include <string.h>

#include "ctest.h"
#include "c_logging/logger.h"

#if defined _MSC_VER && !defined(WINCE)
#include <limits.h> // for SIZE_MAX
#include "windows.h"
#endif

#ifdef USE_VLD
#include "vld.h" // force
#endif

const TEST_FUNCTION_DATA* g_CurrentTestFunction;
jmp_buf g_ExceptionJump;
size_t RunTests(const TEST_FUNCTION_DATA* testListHead, const char* testSuiteName, bool useLeakCheckRetries, const char* testNameFilter)
{
#ifdef USE_VLD
    VLD_UINT initial_leak_count = VLDGetLeaksCount();
#endif
    size_t totalTestCount = 0;
    size_t failedTestCount = 0;
    size_t skippedByFilterCount = 0;
    const TEST_FUNCTION_DATA* currentTestFunction = (const TEST_FUNCTION_DATA*)testListHead->NextTestFunctionData;
    const TEST_FUNCTION_DATA* testSuiteInitialize = NULL;
    const TEST_FUNCTION_DATA* testSuiteCleanup = NULL;
    const TEST_FUNCTION_DATA* testFunctionInitialize = NULL;
    const TEST_FUNCTION_DATA* testFunctionCleanup = NULL;
    int testSuiteInitializeFailed = 0;

#if defined _MSC_VER && !defined(WINCE)
    _set_abort_behavior(_CALL_REPORTFAULT, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    // Set output mode to handle virtual terminal sequences
    HANDLE std_out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    bool SetConsoleMode_succeeded = false;
    DWORD console_mode_initial = 0;
    if (std_out_handle == INVALID_HANDLE_VALUE)
    {
        LogWarning("Error getting console handle, no coloring available. GetLastError()=%" PRIx32 "", (uint32_t)GetLastError());
    }
    else
    {
        if (!GetConsoleMode(std_out_handle, &console_mode_initial))
        {
            LogWarning("Error getting console mode, no coloring available. GetLastError()=%" PRIx32 "", (uint32_t)GetLastError());
        }
        else
        {
            if (!SetConsoleMode(std_out_handle, console_mode_initial | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
            {
                LogWarning("Error setting console mode, no coloring available. GetLastError()=%" PRIx32 "", (uint32_t)GetLastError());
            }
            else
            {
                SetConsoleMode_succeeded = true;
            }
        }
    }

#endif

    g_CurrentTestFunction = NULL;

    LogInfo(" === Executing test suite %s ===", testSuiteName);
    if (testNameFilter != NULL && testNameFilter[0] != '\0')
    {
        LogInfo(" ### Test Filter = %s", testNameFilter);
    }

    while (currentTestFunction->TestFunction != NULL)
    {
        if (currentTestFunction->FunctionType == CTEST_TEST_FUNCTION_INITIALIZE)
        {
            testFunctionInitialize = currentTestFunction;
        }

        if (currentTestFunction->FunctionType == CTEST_TEST_FUNCTION_CLEANUP)
        {
            testFunctionCleanup = currentTestFunction;
        }

        if (currentTestFunction->FunctionType == CTEST_TEST_SUITE_INITIALIZE)
        {
            testSuiteInitialize = currentTestFunction;
        }

        if (currentTestFunction->FunctionType == CTEST_TEST_SUITE_CLEANUP)
        {
            testSuiteCleanup = currentTestFunction;
        }

        currentTestFunction = (TEST_FUNCTION_DATA*)currentTestFunction->NextTestFunctionData;
    }

    if (testSuiteInitialize != NULL)
    {
        if (setjmp(g_ExceptionJump) == 0)
        {
            testSuiteInitialize->TestFunction();
        }
        else
        {
            testSuiteInitializeFailed = 1;
            LogInfo("TEST_SUITE_INITIALIZE failed - suite ending");
        }
    }

    if (testSuiteInitializeFailed == 1)
    {
        /* print results */
        LogInfo(CTEST_ANSI_COLOR_RED "0 tests ran, ALL failed, NONE succeeded." CTEST_ANSI_COLOR_RESET);
        failedTestCount = 1;
    }
    else
    {
        unsigned int is_test_runner_ok = 1;

        currentTestFunction = (const TEST_FUNCTION_DATA*)testListHead->NextTestFunctionData;
        while (currentTestFunction->TestFunction != NULL)
        {
            if (currentTestFunction->FunctionType == CTEST_TEST_FUNCTION)
            {
                /* Check if test should be filtered out */
                if ((testNameFilter != NULL) && (testNameFilter[0] != '\0') && (strcmp(currentTestFunction->TestFunctionName, testNameFilter) != 0))
                {
                    /* Test does not match filter, skip it */
                    *currentTestFunction->TestResult = TEST_SKIPPED_FILTER;
                    skippedByFilterCount++;
                }
                else if (is_test_runner_ok == 1)
                {
                    int testFunctionInitializeFailed = 0;

                    if (testFunctionInitialize != NULL)
                    {
                        if (setjmp(g_ExceptionJump) == 0)
                        {
                            testFunctionInitialize->TestFunction();
                        }
                        else
                        {
                            testFunctionInitializeFailed = 1;
                            LogInfo(CTEST_ANSI_COLOR_RED "TEST_FUNCTION_INITIALIZE failed - next TEST_FUNCTION will fail" CTEST_ANSI_COLOR_RESET);
                        }
                    }

                    if (testFunctionInitializeFailed)
                    {
                        *currentTestFunction->TestResult = TEST_FAILED;
                        LogInfo(CTEST_ANSI_COLOR_YELLOW "Not executing test %s ..." CTEST_ANSI_COLOR_RESET, currentTestFunction->TestFunctionName);
                    }
                    else
                    {
                        LogInfo("Executing test %s ...", currentTestFunction->TestFunctionName);

                        g_CurrentTestFunction = currentTestFunction;

                        if (setjmp(g_ExceptionJump) == 0)
                        {
                            currentTestFunction->TestFunction();
                        }
                        else
                        {
                            /*can only get here if there was a longjmp called while executing currentTestFunction->TestFunction();*/
                            /*we don't do anything*/
                        }
                        g_CurrentTestFunction = NULL;/*g_CurrentTestFunction is limited to actually executing a TEST_FUNCTION, otherwise it should be NULL*/

                        /*in the case when the cleanup can assert... have to prepare the long jump*/
                        if (setjmp(g_ExceptionJump) == 0)
                        {
                            if (testFunctionCleanup != NULL)
                            {
                                testFunctionCleanup->TestFunction();
                            }

                            *currentTestFunction->TestResult = TEST_SUCCESS;
                        }
                        else
                        {
                            /* this is a fatal error, if we got a fail in cleanup we can't do much */
                            *currentTestFunction->TestResult = TEST_FAILED;
                            is_test_runner_ok = 0;
                        }
                    }
                }
                else
                {
                    *currentTestFunction->TestResult = TEST_NOT_EXECUTED;
                }

                if (*currentTestFunction->TestResult == TEST_FAILED)
                {
                    failedTestCount++;
                    LogInfo(CTEST_ANSI_COLOR_RED "Test %s result = !!! FAILED !!!" CTEST_ANSI_COLOR_RESET "", currentTestFunction->TestFunctionName);
                }
                else if (*currentTestFunction->TestResult == TEST_NOT_EXECUTED)
                {
                    failedTestCount++;
                    LogInfo(CTEST_ANSI_COLOR_YELLOW "Test %s ... SKIPPED due to a failure in test function cleanup. " CTEST_ANSI_COLOR_RESET "", currentTestFunction->TestFunctionName);
                }
                else if (*currentTestFunction->TestResult == TEST_SKIPPED_FILTER)
                {
                    LogVerbose(CTEST_ANSI_COLOR_YELLOW "Test %s ... SKIPPED due to filter (%s)." CTEST_ANSI_COLOR_RESET "", currentTestFunction->TestFunctionName, MU_P_OR_NULL(testNameFilter));
                }
                else
                {
                    LogInfo(CTEST_ANSI_COLOR_GREEN "Test %s result = Succeeded." CTEST_ANSI_COLOR_RESET "", currentTestFunction->TestFunctionName);
                }
                totalTestCount++;
            }

            currentTestFunction = (TEST_FUNCTION_DATA*)currentTestFunction->NextTestFunctionData;
        }

        if (setjmp(g_ExceptionJump) == 0)
        {
            if (testSuiteCleanup != NULL)
            {
                testSuiteCleanup->TestFunction();
            }
        }
        else
        {
            /*only get here when testSuiteCleanup did asserted*/
            /*should fail the tests*/
            LogInfo(CTEST_ANSI_COLOR_RED "TEST_SUITE_CLEANUP failed - all tests are marked as failed" CTEST_ANSI_COLOR_RESET "");
            failedTestCount = (totalTestCount > 0) ? totalTestCount : SIZE_MAX;
        }

        /* print results */
        if (skippedByFilterCount > 0)
        {
            LogInfo("%s%d tests ran, %d failed, %d succeeded, %d skipped by filter." CTEST_ANSI_COLOR_RESET "", (failedTestCount > 0) ? (CTEST_ANSI_COLOR_RED) : (CTEST_ANSI_COLOR_GREEN), (int)totalTestCount, (int)failedTestCount, (int)(totalTestCount - failedTestCount), (int)skippedByFilterCount);
        }
        else
        {
            LogInfo("%s%d tests ran, %d failed, %d succeeded." CTEST_ANSI_COLOR_RESET "", (failedTestCount > 0) ? (CTEST_ANSI_COLOR_RED) : (CTEST_ANSI_COLOR_GREEN), (int)totalTestCount, (int)failedTestCount, (int)(totalTestCount - failedTestCount));
        }
    }

#if defined _MSC_VER && !defined(WINCE)
    if (std_out_handle != INVALID_HANDLE_VALUE)
    {
        if (SetConsoleMode_succeeded)
        {
            /*revert console to initial state*/
            if (!SetConsoleMode(std_out_handle, console_mode_initial))
            {
                LogWarning("Error resetting console mode to initial value of %" PRIx32 ". GetLastError()=%" PRIx32 "", console_mode_initial, GetLastError());
            }
        }
    }
#endif

#ifdef USE_VLD
    if (useLeakCheckRetries)
    {
        if (failedTestCount == 0)
        {
            VLD_UINT leaks_count = VLDGetLeaksCount();
            do
            {
                if (leaks_count - initial_leak_count > 0)
                {
                    LogWarning("Leaks count is %u (initial count %u)", leaks_count, initial_leak_count);
                    Sleep(5000);
                    VLD_UINT new_leaks_count = VLDGetLeaksCount();

                    if (new_leaks_count == leaks_count)
                    {
                        // Leaks are stable so there must be real leaks
                        LogWarning("Leaks count has not changed...");
                        break;
                    }
                    else
                    {
                        // Leaks have gone down, try again
                        leaks_count = new_leaks_count;
                    }
                }
                else
                {
                    // No leaks, we are done
                    break;
                }
            } while (1);
        }
    }
    failedTestCount = (failedTestCount > 0) ? failedTestCount : (size_t)(-(int)(VLDGetLeaksCount() - initial_leak_count));
#else
    (void)useLeakCheckRetries;
#endif

    return failedTestCount;
}

static void _Bool_ToString(char* string, size_t bufferSize, int val)
{
    (void)snprintf(string, bufferSize, "%s", val ? "true" : "false");
}

static void int_ToString(char* string, size_t bufferSize, int val)
{
    (void)snprintf(string, bufferSize, "%d", val);
}

static void char_ToString(char* string, size_t bufferSize, char val)
{
    (void)snprintf(string, bufferSize, "%d", (int)val);
}

static void short_ToString(char* string, size_t bufferSize, short val)
{
    (void)snprintf(string, bufferSize, "%d", (int)val);
}

static void long_ToString(char* string, size_t bufferSize, long val)
{
    (void)snprintf(string, bufferSize, "%ld", (long)val);
}

static void size_t_ToString(char* string, size_t bufferSize, size_t val)
{
    (void)snprintf(string, bufferSize, "%d", (int)val);
}

static void float_ToString(char* string, size_t bufferSize, float val)
{
    (void)snprintf(string, bufferSize, "%.02f", val);
}

static void double_ToString(char* string, size_t bufferSize, double val)
{
    (void)snprintf(string, bufferSize, "%.02f", val);
}

static void long_double_ToString(char* string, size_t bufferSize, long_double val)
{
    (void)snprintf(string, bufferSize, "%.02Lf", val);
}

static void char_ptr_ToString(char* string, size_t bufferSize, const char* val)
{
    (void)snprintf(string, bufferSize, "%s", val);
}

static void wchar_ptr_ToString(char* string, size_t bufferSize, const wchar_t* val)
{
    (void)snprintf(string, bufferSize, "%ls", val);
}

static void void_ptr_ToString(char* string, size_t bufferSize, const void* val)
{
    (void)snprintf(string, bufferSize, "%p", val);
}

static void unsigned_long_ToString(char* string, size_t bufferSize, unsigned long val)
{
    (void)snprintf(string, bufferSize, "%lu", val);
}

static int _Bool_Compare(int left, int right)
{
    return left != right;
}

static int int_Compare(int left, int right)
{
    return left != right;
}

static int char_Compare(char left, char right)
{
    return left != right;
}

static int short_Compare(short left, short right)
{
    return left != right;
}

static int long_Compare(long left, long right)
{
    return left != right;
}

static int unsigned_long_Compare(unsigned long left, unsigned long right)
{
    return left != right;
}

static int size_t_Compare(size_t left, size_t right)
{
    return left != right;
}

static int float_Compare(float left, float right)
{
    return left != right;
}

static int double_Compare(double left, double right)
{
    return left != right;
}

static int long_double_Compare(long double left, long double right)
{
    return left != right;
}

static int char_ptr_Compare(const char* left, const char* right)
{
    if ((left == NULL) &&
        (right == NULL))
    {
        return 0;
    }
    else if (left == NULL)
    {
        return -1;
    }
    else if (right == NULL)
    {
        return 1;
    }
    else
    {
        return strcmp(left, right);
    }
}

static int wchar_ptr_Compare(const wchar_t* left, const wchar_t* right)
{
    if ((left == NULL) &&
        (right == NULL))
    {
        return 0;
    }
    else if (left == NULL)
    {
        return -1;
    }
    else if (right == NULL)
    {
        return 1;
    }
    else
    {
        return wcscmp(left, right);
    }
}

static int void_ptr_Compare(const void* left, const void* right)
{
    return left != right;
}

#if defined CTEST_USE_STDINT
static void uint8_t_ToString(char* string, size_t bufferSize, uint8_t val)
{
    (void)snprintf(string, bufferSize, "%"PRIu8, val);
}

static void int8_t_ToString(char* string, size_t bufferSize, int8_t val)
{
    (void)snprintf(string, bufferSize, "%"PRId8, val);
}

static void uint16_t_ToString(char* string, size_t bufferSize, uint16_t val)
{
    (void)snprintf(string, bufferSize, "%"PRIu16, val);
}

static void int16_t_ToString(char* string, size_t bufferSize, int16_t val)
{
    (void)snprintf(string, bufferSize, "%"PRId16, val);
}

static void uint32_t_ToString(char* string, size_t bufferSize, uint32_t val)
{
    (void)snprintf(string, bufferSize, "%"PRIu32, val);
}

static void int32_t_ToString(char* string, size_t bufferSize, int32_t val)
{
    (void)snprintf(string, bufferSize, "%"PRId32, val);
}

static void uint64_t_ToString(char* string, size_t bufferSize, uint64_t val)
{
    (void)snprintf(string, bufferSize, "%"PRIu64, val);
}

static void int64_t_ToString(char* string, size_t bufferSize, int64_t val)
{
    (void)snprintf(string, bufferSize, "%"PRId64, val);
}

static int uint8_t_Compare(uint8_t left, uint8_t right)
{
    return left != right;
}

static int int8_t_Compare(int8_t left, int8_t right)
{
    return left != right;
}

static int uint16_t_Compare(uint16_t left, uint16_t right)
{
    return left != right;
}

static int int16_t_Compare(int16_t left, int16_t right)
{
    return left != right;
}

static int uint32_t_Compare(uint32_t left, uint32_t right)
{
    return left != right;
}

static int int32_t_Compare(int32_t left, int32_t right)
{
    return left != right;
}

static int uint64_t_Compare(uint64_t left, uint64_t right)
{
    return left != right;
}

static int int64_t_Compare(int64_t left, int64_t right)
{
    return left != right;
}

#endif

void bool_AssertAreEqual(int left, int right, char* ctest_message, int line_no)
{
    CTEST_EQUALITY_ASSERT_IMPL_FOR_TYPE(_Bool, true, line_no)
}

void _Bool_AssertAreEqual(int left, int right, char* ctest_message, int line_no)
{
    CTEST_EQUALITY_ASSERT_IMPL_FOR_TYPE(_Bool, true, line_no)
}

void bool_AssertAreNotEqual(int left, int right, char* ctest_message, int line_no)
{
    CTEST_EQUALITY_ASSERT_IMPL_FOR_TYPE(_Bool, false, line_no)
}

void _Bool_AssertAreNotEqual(int left, int right, char* ctest_message, int line_no)
{
    CTEST_EQUALITY_ASSERT_IMPL_FOR_TYPE(_Bool, false, line_no)
}

CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(int,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(char,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(short,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(long,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(size_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(float,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(double,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(long_double,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(char_ptr,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(wchar_ptr,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(void_ptr,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(unsigned_long,)

#if defined CTEST_USE_STDINT
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(uint8_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(int8_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(uint16_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(int16_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(uint32_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(int32_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(uint64_t,)
CTEST_DEFINE_EQUALITY_ASSERTION_FUNCTIONS_FOR_TYPE(int64_t,)
#endif

static char* ctest_vsprintf_char(const char* format, va_list va)
{
    char* result;
    va_list va_clone;
    va_copy(va_clone, va);  /*this is needed because "va" would be passed twice to a function that parses it with va_arg: once in vsnprintf(NULL, 0, format, va) and the second time in vsnprintf(result, neededSize + 1, format, va) yielding undefined behavior*/
    int neededSize = vsnprintf(NULL, 0, format, va_clone);
    if (neededSize < 0)
    {
        LogError("failure in vsnprintf, format=%s, va_clone=%p);", format, (void*)&va_clone);
        result = NULL;
    }
    else
    {
        result = malloc(neededSize + 1);
        if (result == NULL)
        {
            LogError("failure in malloc");
            /*return as is*/
        }
        else
        {
            if (vsnprintf(result, neededSize + 1, format, va) != neededSize)
            {
                LogError("inconsistent vsnprintf behavior format, neededSize=%d + 1, format=%s, va=%p", neededSize, format, (void*)&va);
                free(result);
                result = NULL;
            }
        }
    }
    va_end(va_clone);
    return result;
}

/*returns a char* that is as if printed by printf*/
/*needs to be free'd after usage*/
char* ctest_sprintf_char(const char* format, ...)
{
    char* result;
    va_list va;
    va_start(va, format);
    result = ctest_vsprintf_char(format, va);
    va_end(va);
    return result;
}

void ctest_sprintf_free(char* string)
{
    free(string);
}

void do_jump(jmp_buf *exceptionJump, const volatile void* expected, const volatile void* actual)
{
    /*setting a breakpoint here allows catching the jump before it happens*/
    (void)expected;
    (void)actual;
#if CTEST_ABORT_ON_FAIL
    (void)exceptionJump;
    abort();
#else
    longjmp(*exceptionJump, 0xca1e4);
#endif
}
