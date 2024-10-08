#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(ctest_ut_c_files
    assert_failures_with_msg_tests.c
    assertfailurestests.c
    assertsuccesstests.c
    ctestunittests.c
    enum_define_tests.c
    simpletestsuiteonetest.c
    simpletestsuitetwotests.c
    testfunctioncleanuptests.c
    testfunctioninitializetests.c
    testsuiteinitializecleanuptests.c
    whentestsuiteinitializefailstests.c
    whentestfunctioninitializefailstests.c
    testfunctioncleanupfailstests.c
    testsuitecleanuptests.c
    testsuitecleanuptests2.c
)

if (MSVC)
set(ctest_ut_c_files
    ${ctest_ut_c_files}
    windows_types_tests.c
)
endif()

set(ctest_ut_cpp_files
    assertsuccesstestscpp.cpp
    simpletestsuiteonetestcpp.cpp
)

set(ctest_ut_h_files
)

add_executable(ctest_ut ${ctest_ut_c_files} ${ctest_ut_cpp_files} ${ctest_ut_h_files})

set_target_properties(ctest_ut
               PROPERTIES
               FOLDER "tests/ctest")

target_link_libraries(ctest_ut ctest)

if(${run_unittests})
    add_test(NAME ctest_ut COMMAND ctest_ut)
endif()
