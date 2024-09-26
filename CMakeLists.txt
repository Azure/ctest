#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if(NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
    cmake_minimum_required(VERSION 3.18)
endif()

# canon way of using ctest from another repo is below. It assumes the using repo has placed ctest in "deps"

#if ((NOT TARGET ctest) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/ctest/CMakeLists.txt))
#    add_subdirectory(deps/ctest)
#    include_directories(${CTEST_INC_FOLDER})
#endif() 

if(TARGET ctest)
    RETURN()
endif()

project(ctest)

set(CTEST_VERSION 1.1.19)

include (CheckIncludeFiles)

CHECK_INCLUDE_FILES(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES(stdbool.h HAVE_STDBOOL_H)

#Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Build with -fPIC always
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

option(run_unittests "set run_unittests to ON to run unittests (default is OFF)" OFF)
if ((MSVC) OR (UNIX) OR (LINUX))
# Enable coloring by default for Linux, *nix and Windows
option(use_coloring "use test coloring (default is ON)" ON)
else()
option(use_coloring "use test coloring (default is OFF)" OFF)
endif()
option(abort_on_fail "abort on test failure (default is OFF)" OFF)


#bring in dependencies
#do not add or build any tests of the dependencies
set(original_run_e2e_tests ${run_e2e_tests})
set(original_run_int_tests ${run_int_tests})
set(original_run_perf_tests ${run_perf_tests})
set(original_run_unittests ${run_unittests})
set(original_run_traceability ${run_traceability})
set(original_run_reals_check ${run_reals_check})

set(run_e2e_tests OFF)
set(run_int_tests OFF)
set(run_perf_tests OFF)
set(run_unittests OFF)
set(run_traceability OFF)
set(run_reals_check OFF)

if ((NOT TARGET c_build_tools) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-build-tools/CMakeLists.txt))
    add_subdirectory(deps/c-build-tools)
    set_default_build_options()
endif() 

if ((NOT TARGET macro_utils_c) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/macro-utils-c/CMakeLists.txt))
    add_subdirectory(deps/macro-utils-c)
    include_directories(${MACRO_UTILS_INC_FOLDER})
endif() 

if ((NOT TARGET c_logging) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/c-logging/CMakeLists.txt))
    add_subdirectory(deps/c-logging)
    include_directories(deps/c-logging/v2/inc)
endif() 

set(run_e2e_tests ${original_run_e2e_tests})
set(run_int_tests ${original_run_int_tests})
set(run_perf_tests ${original_run_perf_tests})
set(run_unittests ${original_run_unittests})
set(run_traceability ${original_run_traceability})
set(run_reals_check ${original_run_reals_check})

include (CTest)

if(${use_coloring})
    add_definitions(-DUSE_COLORING)
endif()

if(${abort_on_fail})
    add_definitions(-DCTEST_ABORT_ON_FAIL)
endif()

set(ctest_c_files
    ./src/ctest.c
)

set(ctest_h_files
    ./inc/ctest.h
)

if (MSVC)
set(ctest_c_files
    ${ctest_c_files}
    ./src/ctest_windows.c
)

set(ctest_h_files
    ${ctest_h_files}
    ./inc/ctest_windows.h
)
endif()

#these are the include folders
#the following "set" statetement exports across the project a global variable called CTEST_INC_FOLDER that expands to whatever needs to included when using ctest library
set(CTEST_INC_FOLDER ${CMAKE_CURRENT_LIST_DIR}/inc CACHE INTERNAL "this is what needs to be included if using ctest lib" FORCE)

include_directories(${CTEST_INC_FOLDER})

if ((NOT HAVE_STDINT_H) OR (NOT HAVE_STDBOOL_H))
    include_directories(${CTEST_INC_FOLDER}/aux_inc)
endif()

add_library(
ctest ${ctest_c_files} ${ctest_h_files}
)

target_include_directories(ctest
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(ctest PRIVATE c_logging_v2)

set_target_properties(ctest
               PROPERTIES
               FOLDER "test_tools")

if (${run_unittests})
     add_subdirectory(tests)
endif()

if(${run_reals_check})
    add_reals_check_target()
endif()

#Insert vld in all executables if so required
add_vld_if_defined(${CMAKE_CURRENT_SOURCE_DIR})