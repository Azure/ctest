#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

# Integration tests for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
# The verdict is the process exit code, so each scenario is a separate executable. They cover clean
# allocations, process-lifetime globals (in the executable and in DLLs), and genuine leaks; the
# executables that leak in a way the check can detect are registered WILL_FAIL.

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

# Helper modules for the global / DLL allocation variants. Each module instance owns one
# process-lifetime global that allocates on construction and frees on destruction, plus an exported
# function that leaks on purpose. VLD only tracks allocations made by a module that links VLD, so the
# helper DLL is instrumented below; otherwise its allocations would be invisible to the leak check.
add_library(ctest_leak_check_helper_dll SHARED leak_check_helper.cpp leak_check_helper.h)
target_compile_definitions(ctest_leak_check_helper_dll PRIVATE LEAK_CHECK_HELPER_EXPORTS)
set_target_properties(ctest_leak_check_helper_dll PROPERTIES FOLDER "tests/ctest")

add_library(ctest_leak_check_helper_static STATIC leak_check_helper.cpp leak_check_helper.h)
target_compile_definitions(ctest_leak_check_helper_static PUBLIC LEAK_CHECK_HELPER_STATIC)
set_target_properties(ctest_leak_check_helper_static PROPERTIES FOLDER "tests/ctest")

# Same helper as a DLL but built with the static CRT, giving it a heap separate from the /MD test
# executable. It is deliberately not VLD-instrumented (see below), so it lets the tests show what a
# static-CRT DLL looks like to the leak check.
add_library(ctest_leak_check_helper_dll_mt SHARED leak_check_helper.cpp leak_check_helper.h)
target_compile_definitions(ctest_leak_check_helper_dll_mt PRIVATE LEAK_CHECK_HELPER_EXPORTS)
set_target_properties(ctest_leak_check_helper_dll_mt PROPERTIES
    FOLDER "tests/ctest"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# VLD only tracks allocations made by a module that itself links VLD. The test executables get VLD
# via add_vld_if_defined (executables only), so the /MD helper DLL is instrumented explicitly or its
# allocations would be invisible to the leak check. The static helper needs nothing (it is linked
# into an executable that already carries VLD). The /MT helper cannot be instrumented: use_vld pulls
# the /MD CRT and conflicts with the static CRT (LNK4098), so leaks in a static-CRT DLL stay
# invisible -- which the dll_mt_real_leak test documents.
if(${use_vld})
    add_use_vld_if_no_cs(ctest_leak_check_helper_dll)
endif()

add_executable(ctest_leak_check_namespace_global_int ctest_leak_check_namespace_global_int.cpp ctest_leak_check_namespace_global_int_main.c)
target_link_libraries(ctest_leak_check_namespace_global_int ctest)
set_target_properties(ctest_leak_check_namespace_global_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_global_loadtime_int ctest_leak_check_dll_global_loadtime_int.cpp ctest_leak_check_dll_global_loadtime_int_main.c)
target_link_libraries(ctest_leak_check_dll_global_loadtime_int ctest ctest_leak_check_helper_dll)
set_target_properties(ctest_leak_check_dll_global_loadtime_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_global_loadlibrary_int ctest_leak_check_dll_global_loadlibrary_int.cpp ctest_leak_check_dll_global_loadlibrary_int_main.c)
target_link_libraries(ctest_leak_check_dll_global_loadlibrary_int ctest)
add_dependencies(ctest_leak_check_dll_global_loadlibrary_int ctest_leak_check_helper_dll)
set_target_properties(ctest_leak_check_dll_global_loadlibrary_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_global_never_freed_int ctest_leak_check_dll_global_never_freed_int.cpp ctest_leak_check_dll_global_never_freed_int_main.c)
target_link_libraries(ctest_leak_check_dll_global_never_freed_int ctest)
add_dependencies(ctest_leak_check_dll_global_never_freed_int ctest_leak_check_helper_dll)
set_target_properties(ctest_leak_check_dll_global_never_freed_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_dual_link_int ctest_leak_check_dll_dual_link_int.cpp ctest_leak_check_dll_dual_link_int_main.c)
target_link_libraries(ctest_leak_check_dll_dual_link_int ctest ctest_leak_check_helper_static)
add_dependencies(ctest_leak_check_dll_dual_link_int ctest_leak_check_helper_dll_mt)
set_target_properties(ctest_leak_check_dll_dual_link_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_real_leak_int ctest_leak_check_dll_real_leak_int.cpp ctest_leak_check_dll_real_leak_int_main.c)
target_link_libraries(ctest_leak_check_dll_real_leak_int ctest)
add_dependencies(ctest_leak_check_dll_real_leak_int ctest_leak_check_helper_dll)
set_target_properties(ctest_leak_check_dll_real_leak_int PROPERTIES FOLDER "tests/ctest")

add_executable(ctest_leak_check_dll_mt_real_leak_int ctest_leak_check_dll_mt_real_leak_int.cpp ctest_leak_check_dll_mt_real_leak_int_main.c)
target_link_libraries(ctest_leak_check_dll_mt_real_leak_int ctest)
add_dependencies(ctest_leak_check_dll_mt_real_leak_int ctest_leak_check_helper_dll_mt)
set_target_properties(ctest_leak_check_dll_mt_real_leak_int PROPERTIES FOLDER "tests/ctest")

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

    # Process-lifetime allocations that survive to the at-exit check only because they are correctly
    # destroyed during teardown (global in this executable, global in a VLD-instrumented DLL whether
    # load-time linked, LoadLibrary'd and freed, or LoadLibrary'd and never freed) must not be
    # reported. The dual-link case adds a statically linked copy plus a separately loaded static-CRT
    # DLL. All exit 0.
    add_test(NAME ctest_leak_check_namespace_global_int COMMAND ctest_leak_check_namespace_global_int CONFIGURATIONS Debug)
    add_test(NAME ctest_leak_check_dll_global_loadtime_int COMMAND ctest_leak_check_dll_global_loadtime_int CONFIGURATIONS Debug)
    add_test(NAME ctest_leak_check_dll_global_loadlibrary_int COMMAND ctest_leak_check_dll_global_loadlibrary_int CONFIGURATIONS Debug)
    add_test(NAME ctest_leak_check_dll_global_never_freed_int COMMAND ctest_leak_check_dll_global_never_freed_int CONFIGURATIONS Debug)
    add_test(NAME ctest_leak_check_dll_dual_link_int COMMAND ctest_leak_check_dll_dual_link_int CONFIGURATIONS Debug)

    # A genuine leak in a VLD-instrumented (/MD) DLL is detected and fails the run.
    add_test(NAME ctest_leak_check_dll_real_leak_int COMMAND ctest_leak_check_dll_real_leak_int CONFIGURATIONS Debug)
    set_tests_properties(ctest_leak_check_dll_real_leak_int PROPERTIES WILL_FAIL TRUE)

    # A genuine leak in a static-CRT (/MT) DLL is NOT detected: that DLL cannot be VLD-instrumented
    # (use_vld pulls the /MD CRT and conflicts, LNK4098), so the allocation is invisible. This test
    # documents that limitation, so it passes only while such leaks remain undetected (exit 0).
    add_test(NAME ctest_leak_check_dll_mt_real_leak_int COMMAND ctest_leak_check_dll_mt_real_leak_int CONFIGURATIONS Debug)
endif()
