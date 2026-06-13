#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

# Integration tests for ctest's at-exit VLD leak check (ctest_check_leaks_at_exit in src/ctest.c).
# The verdict is the process exit code, so each scenario is a separate executable. They cover clean
# allocations, process-lifetime globals (in the executable and in DLLs), and genuine leaks.

# Builds a leak-check test executable and, when the leak check is active, registers it. The
# executable is always built so it gets compile coverage in every flavor; the test is registered only
# when use_vld is enabled together with unit or integration tests, and only for the Debug
# configuration, because VLD's leak-counting API is a no-op under NDEBUG (RelWithDebInfo/Release).
# SOURCES defaults to <name>.c; extra link libraries and build-order dependencies are passed with
# LIBS and DEPENDS. ctest_leak_check_passing_test expects the process to exit 0 (no leak reported);
# ctest_leak_check_failing_test marks the test WILL_FAIL (a leak the check detects, so it must exit
# non-zero).
function(ctest_leak_check_test name will_fail)
    cmake_parse_arguments(arg "" "" "SOURCES;LIBS;DEPENDS" ${ARGN})
    if(NOT arg_SOURCES)
        set(arg_SOURCES ${name}.c)
    endif()
    add_executable(${name} ${arg_SOURCES})
    target_link_libraries(${name} ctest ${arg_LIBS})
    if(arg_DEPENDS)
        add_dependencies(${name} ${arg_DEPENDS})
    endif()
    set_target_properties(${name} PROPERTIES FOLDER "tests/ctest")
    if((${run_unittests} OR ${run_int_tests}) AND ${use_vld})
        add_test(NAME ${name} COMMAND ${name} CONFIGURATIONS Debug)
        if(will_fail)
            set_tests_properties(${name} PROPERTIES WILL_FAIL TRUE)
        endif()
    endif()
endfunction()

function(ctest_leak_check_passing_test name)
    ctest_leak_check_test(${name} FALSE ${ARGN})
endfunction()

function(ctest_leak_check_failing_test name)
    ctest_leak_check_test(${name} TRUE ${ARGN})
endfunction()

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

# Clean allocations and process-lifetime globals (in the executable and in VLD-instrumented DLLs,
# whether load-time linked, LoadLibrary'd and freed, or LoadLibrary'd and never freed) are destroyed
# during teardown and must not be reported: they exit 0.
ctest_leak_check_passing_test(ctest_leak_check_clean_int)
ctest_leak_check_passing_test(ctest_leak_check_process_lifetime_static_int
    SOURCES ctest_leak_check_process_lifetime_static_int.cpp ctest_leak_check_process_lifetime_static_int_main.c)
ctest_leak_check_passing_test(ctest_leak_check_namespace_global_int
    SOURCES ctest_leak_check_namespace_global_int.cpp ctest_leak_check_namespace_global_int_main.c)
ctest_leak_check_passing_test(ctest_leak_check_dll_global_loadtime_int
    SOURCES ctest_leak_check_dll_global_loadtime_int.cpp ctest_leak_check_dll_global_loadtime_int_main.c
    LIBS ctest_leak_check_helper_dll)
ctest_leak_check_passing_test(ctest_leak_check_dll_global_loadlibrary_int
    SOURCES ctest_leak_check_dll_global_loadlibrary_int.cpp ctest_leak_check_dll_global_loadlibrary_int_main.c
    DEPENDS ctest_leak_check_helper_dll)
ctest_leak_check_passing_test(ctest_leak_check_dll_global_never_freed_int
    SOURCES ctest_leak_check_dll_global_never_freed_int.cpp ctest_leak_check_dll_global_never_freed_int_main.c
    DEPENDS ctest_leak_check_helper_dll)
# The dual-link case adds a statically linked copy plus a separately loaded static-CRT DLL.
ctest_leak_check_passing_test(ctest_leak_check_dll_dual_link_int
    SOURCES ctest_leak_check_dll_dual_link_int.cpp ctest_leak_check_dll_dual_link_int_main.c
    LIBS ctest_leak_check_helper_static DEPENDS ctest_leak_check_helper_dll_mt)
# A genuine leak in a static-CRT (/MT) DLL is NOT detected (that DLL cannot be VLD-instrumented), so
# the leak is invisible and the test exits 0, documenting that limitation.
ctest_leak_check_passing_test(ctest_leak_check_dll_mt_real_leak_int
    SOURCES ctest_leak_check_dll_mt_real_leak_int.cpp ctest_leak_check_dll_mt_real_leak_int_main.c
    DEPENDS ctest_leak_check_helper_dll_mt)

# Genuine leaks the check can detect must fail the run.
ctest_leak_check_failing_test(ctest_leak_check_real_leak_int)
ctest_leak_check_failing_test(ctest_leak_check_real_leak_cpp_int
    SOURCES ctest_leak_check_real_leak_cpp_int.cpp ctest_leak_check_real_leak_cpp_int_main.c)
ctest_leak_check_failing_test(ctest_leak_check_dll_real_leak_int
    SOURCES ctest_leak_check_dll_real_leak_int.cpp ctest_leak_check_dll_real_leak_int_main.c
    DEPENDS ctest_leak_check_helper_dll)
