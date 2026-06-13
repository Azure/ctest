#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if (${run_unittests})
add_subdirectory(ctest_ut)
add_subdirectory(ctest_macro_hooks_ut)
add_subdirectory(ctest_custom_fixtures_ut)
add_subdirectory(ctest_parameterized_ut)
endif()

if (${run_int_tests})
# The leak-check integration tests exercise the VLD-based leak check, which is Windows/MSVC only
# (the helpers also use __declspec and LoadLibrary), so they are excluded on other platforms.
if(WIN32)
    add_subdirectory(ctest_leak_check_int)
endif()
endif()