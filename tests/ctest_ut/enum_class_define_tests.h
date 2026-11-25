// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef ENUM_CLASS_DEFINE_TESTS_H
#define ENUM_CLASS_DEFINE_TESTS_H

#ifdef __cplusplus

/*stands in for an enum class owned by an external C++ SDK*/
namespace external_sdk
{
    enum class test_enum_class
    {
        kFirst = 3,
        kSecond = 7
    };
}

using TEST_ENUM_CLASS = external_sdk::test_enum_class;

#define TEST_ENUM_CLASS_VALUES \
    kFirst, \
    kSecond

#endif /*__cplusplus*/

#endif /*ENUM_CLASS_DEFINE_TESTS_H*/
