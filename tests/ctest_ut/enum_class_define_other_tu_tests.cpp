// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "ctest.h"

#include "enum_class_define_tests.h"

/*TEST_ENUM_CLASS is also registered in enum_class_define_tests.cpp, which is linked into the same executable*/
CTEST_DEFINE_ENUM_CLASS_TYPE(TEST_ENUM_CLASS, TEST_ENUM_CLASS_VALUES)

CTEST_BEGIN_TEST_SUITE(enum_class_define_other_tu_tests)

CTEST_FUNCTION(CTEST_ASSERT_ARE_EQUAL_with_enum_class_registered_in_two_translation_units_succeeds)
{
    // act
    CTEST_ASSERT_ARE_EQUAL(TEST_ENUM_CLASS, TEST_ENUM_CLASS::kSecond, TEST_ENUM_CLASS::kSecond);
}

CTEST_END_TEST_SUITE(enum_class_define_other_tu_tests)
