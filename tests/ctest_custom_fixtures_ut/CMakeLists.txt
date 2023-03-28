#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

set(ctest_custom_fixtures_ut_c_files
    ctest_one_custom_fixture_ut.c
    ctest_multiple_custom_fixtures_ut.c
    main.c
)

set(ctest_custom_fixtures_ut_h_files
    ctest_one_custom_fixture_ut.h
    ctest_multiple_custom_fixtures_ut.h
)

include_directories(${CTEST_INC_FOLDER})

add_executable(ctest_custom_fixtures_ut ${ctest_custom_fixtures_ut_c_files} ${ctest_custom_fixtures_ut_h_files})

set_target_properties(ctest_custom_fixtures_ut
               PROPERTIES
               FOLDER "tests/ctest")

target_link_libraries(ctest_custom_fixtures_ut ctest)

if(${run_unittests})
	add_test(NAME ctest_custom_fixtures_ut COMMAND ctest_custom_fixtures_ut)
endif()
