#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

# These integration tests exercise ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in
# src/ctest.c). They are meaningful only when VLD is enabled (use_vld), since the leak check is
# compiled out otherwise. Each scenario is a separate executable because the verdict is the process
# exit code, decided during process teardown (the at-exit handler forces a non-zero exit on a real
# leak). The two "real_leak" executables leak on purpose and are therefore registered WILL_FAIL.

add_executable(ctest_leak_check_clean_int ctest_leak_check_clean_int.c)
target_link_libraries(ctest_leak_check_clean_int ctest)
set_target_properties(ctest_leak_check_clean_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_real_leak_int ctest_leak_check_real_leak_int.c)
target_link_libraries(ctest_leak_check_real_leak_int ctest)
set_target_properties(ctest_leak_check_real_leak_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_process_lifetime_static_int ctest_leak_check_process_lifetime_static_int.cpp ctest_leak_check_process_lifetime_static_int_main.c)
target_link_libraries(ctest_leak_check_process_lifetime_static_int ctest)
set_target_properties(ctest_leak_check_process_lifetime_static_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_real_leak_cpp_int ctest_leak_check_real_leak_cpp_int.cpp ctest_leak_check_real_leak_cpp_int_main.c)
target_link_libraries(ctest_leak_check_real_leak_cpp_int ctest)
set_target_properties(ctest_leak_check_real_leak_cpp_int PROPERTIES FOLDER "tests/ctest")

# The at-exit leak check only runs under VLD, so only register these tests when VLD is enabled.
if((run_unittests OR run_int_tests) AND use_vld)
    add_test(NAME ctest_leak_check_clean_int COMMAND ctest_leak_check_clean_int)
    add_test(NAME ctest_leak_check_process_lifetime_static_int COMMAND ctest_leak_check_process_lifetime_static_int)

    add_test(NAME ctest_leak_check_real_leak_int COMMAND ctest_leak_check_real_leak_int)
    set_tests_properties(ctest_leak_check_real_leak_int PROPERTIES WILL_FAIL TRUE)

    add_test(NAME ctest_leak_check_real_leak_cpp_int COMMAND ctest_leak_check_real_leak_cpp_int)
    set_tests_properties(ctest_leak_check_real_leak_cpp_int PROPERTIES WILL_FAIL TRUE)
endif()
