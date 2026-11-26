// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "ctest.h"

#include "enum_class_define_tests.h"

CTEST_DEFINE_ENUM_CLASS_TYPE(TEST_ENUM_CLASS, TEST_ENUM_CLASS_VALUES)

/*the test ending in _fails is expected to fail; ctestunittests.c checks the failure count*/
CTEST_BEGIN_TEST_SUITE(enum_class_define_tests)

CTEST_FUNCTION(CTEST_DEFINE_ENUM_CLASS_TYPE_ToString_writes_the_enumerator_name)
{
    // arrange
    char value_as_string[32];

    // act
    TEST_ENUM_CLASS_ToString(value_as_string, sizeof(value_as_string), TEST_ENUM_CLASS::kSecond);

    // assert
    CTEST_ASSERT_ARE_EQUAL(char_ptr, "kSecond", value_as_string);
}

CTEST_FUNCTION(CTEST_ASSERT_ARE_EQUAL_with_equal_enum_class_values_succeeds)
{
    // act
    CTEST_ASSERT_ARE_EQUAL(TEST_ENUM_CLASS, TEST_ENUM_CLASS::kFirst, TEST_ENUM_CLASS::kFirst);
}

CTEST_FUNCTION(CTEST_ASSERT_ARE_EQUAL_with_different_enum_class_values_fails)
{
    // act
    CTEST_ASSERT_ARE_EQUAL(TEST_ENUM_CLASS, TEST_ENUM_CLASS::kFirst, TEST_ENUM_CLASS::kSecond);
}

CTEST_END_TEST_SUITE(enum_class_define_tests)
