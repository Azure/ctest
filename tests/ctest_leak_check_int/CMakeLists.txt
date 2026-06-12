#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

# Integration tests for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
# The verdict is the process exit code, so each scenario is a separate executable; the two real_leak
# executables leak on purpose and are registered WILL_FAIL.

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

# Register only under VLD, and only for the Debug configuration: VLD's leak-counting API no-ops under
# NDEBUG (RelWithDebInfo/Release), so leak detection -- and the WILL_FAIL real-leak cases in
# particular -- is only meaningful with the debug CRT. CONFIGURATIONS keeps this correct for
# multi-config generators, where the same configured tree runs both Debug and RelWithDebInfo.
if((run_unittests OR run_int_tests) AND use_vld)
    add_test(NAME ctest_leak_check_clean_int COMMAND ctest_leak_check_clean_int CONFIGURATIONS Debug)
    add_test(NAME ctest_leak_check_process_lifetime_static_int COMMAND ctest_leak_check_process_lifetime_static_int CONFIGURATIONS Debug)

    add_test(NAME ctest_leak_check_real_leak_int COMMAND ctest_leak_check_real_leak_int CONFIGURATIONS Debug)
    set_tests_properties(ctest_leak_check_real_leak_int PROPERTIES WILL_FAIL TRUE)

    add_test(NAME ctest_leak_check_real_leak_cpp_int COMMAND ctest_leak_check_real_leak_cpp_int CONFIGURATIONS Debug)
    set_tests_properties(ctest_leak_check_real_leak_cpp_int PROPERTIES WILL_FAIL TRUE)
endif()
