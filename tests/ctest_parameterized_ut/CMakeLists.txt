#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(ctest_parameterized_ut_c_files
    ctest_parameterized_ut.c
    main.c
)

set(ctest_parameterized_ut_h_files
    ctest_parameterized_ut.h
)

add_executable(ctest_parameterized_ut ${ctest_parameterized_ut_c_files} ${ctest_parameterized_ut_h_files})

set_target_properties(ctest_parameterized_ut
               PROPERTIES
               FOLDER "tests/ctest")

target_link_libraries(ctest_parameterized_ut ctest)

if(${run_unittests})
    add_test(NAME ctest_parameterized_ut COMMAND ctest_parameterized_ut)
endif()
