// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <stdio.h>   // for snprintf, NULL

#include "ctest.h"

CTEST_BEGIN_TEST_SUITE(AssertSuccessTestsCpp)

struct MyStruct
{
    // Presence of user-defined constructor
    // disables default contructor. This
    // ensured the CTEST API works correctly
    // with types that can't be default constructed.
    MyStruct(int i):
        x{i} {}

    int x;
};

static MyStruct A(1);
static MyStruct B(2);

CTEST_COMPARE(MyStruct, MyStruct)
{
    return (left.x != right.x);
}

CTEST_TO_STRING(MyStruct, MyStruct, string, bufferSize, value)
{
    (void)snprintf(string, bufferSize, "{ %d }", (int)value.x);
}

CTEST_COMPARE(MyStruct_ref, MyStruct&)
{
    return (left.x != right.x);
}

CTEST_TO_STRING(MyStruct_ref, MyStruct&, string, bufferSize, value)
{
    (void)snprintf(string, bufferSize, "{ %d }", (int)value.x);
}

// Make sure bool also works in C++ since boolean
// types have subtle variations between C and C++.

CTEST_FUNCTION(Assert_Are_Equal_2_Bools_Succeeds)
{
    CTEST_ASSERT_ARE_EQUAL(bool, true, true);
}

CTEST_FUNCTION(Assert_Are_Not_Equal_2_Bools_Succeeds)
{
    CTEST_ASSERT_ARE_NOT_EQUAL(bool, true, false);
}

// Types without default constructors

CTEST_FUNCTION(Assert_Are_Equal_2_User_Types)
{
    CTEST_ASSERT_ARE_EQUAL(MyStruct, A, A);
}

CTEST_FUNCTION(Assert_Are_Not_Equal_2_User_Types)
{
    CTEST_ASSERT_ARE_NOT_EQUAL(MyStruct, A, B);
}

// References

CTEST_FUNCTION(Assert_Are_Equal_2_User_Type_References)
{
    CTEST_ASSERT_ARE_EQUAL(MyStruct_ref, A, A);
}

CTEST_FUNCTION(Assert_Are_Not_Equal_2_User_Type_References)
{
    CTEST_ASSERT_ARE_NOT_EQUAL(MyStruct_ref, A, B);
}

CTEST_END_TEST_SUITE(AssertSuccessTestsCpp)
