#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(ctest_macro_hooks_ut_c_files
    ctest_macro_hooks_ut.c
    main.c
)

set(ctest_macro_hooks_ut_h_files
    ctest_test_hooks.h
)

add_executable(ctest_macro_hooks_ut ${ctest_macro_hooks_ut_c_files} ${ctest_macro_hooks_ut_h_files})

set_target_properties(ctest_macro_hooks_ut
               PROPERTIES
               FOLDER "tests/ctest")

target_link_libraries(ctest_macro_hooks_ut ctest)

if(${run_unittests})
    add_test(NAME ctest_macro_hooks_ut COMMAND ctest_macro_hooks_ut)
endif()
