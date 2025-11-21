# Copilot Instructions for ctest

## Overview
ctest is a portable C test runner for Azure C libraries. **Not related to CMake's ctest**. It provides macro-based test suites with comprehensive assertion support, Windows-specific features, and integration with Azure DevOps pipelines.

## External Dependencies and Standards
All code must follow the comprehensive coding standards defined in #file:../deps/c-build-tools/.github/general_coding_instructions.md. For detailed patterns and conventions, refer to dependency-specific instructions:

- **Build Infrastructure**: #file:../deps/c-build-tools/.github/copilot-instructions.md
- **Macro Utilities**: #file:../deps/macro-utils-c/.github/copilot-instructions.md
- **Logging Framework**: #file:../deps/c-logging/.github/copilot-instructions.md

## Architecture & Key Components

### Core Test Framework
- **`inc/ctest.h`**: Main API with macro-based test definitions and assertion functions
- **`inc/ctest_windows.h`**: Windows-specific types (HRESULT, ULONG64, etc.) for Windows platform testing
- **`src/ctest.c`**: Test runner implementation with colored output, fixture support, and VLD integration
- **Macro System**: Heavily uses `macro-utils-c` for test function generation and linking

### Test Suite Structure
```c
CTEST_BEGIN_TEST_SUITE(SuiteName)

CTEST_FUNCTION(TestName)
{
    // arrange
    // act 
    // assert
    CTEST_ASSERT_ARE_EQUAL(int, expected, actual);
}

CTEST_END_TEST_SUITE(SuiteName)
```

### Running Tests
```c
// Basic execution
CTEST_RUN_TEST_SUITE(SuiteName);

// With leak detection retries (VLD support)
CTEST_RUN_TEST_SUITE_WITH_LEAK_CHECK_RETRIES(SuiteName);
```

## Project-Specific Patterns

### Fixture System
```c
// Suite-level setup/teardown
CTEST_SUITE_INITIALIZE(funcName, optional_fixture_functions...)
CTEST_SUITE_CLEANUP(funcName, optional_fixture_functions...)

// Test-level setup/teardown  
CTEST_FUNCTION_INITIALIZE(funcName, optional_fixture_functions...)
CTEST_FUNCTION_CLEANUP(funcName, optional_fixture_functions...)
```

### Custom Type Assertions
```c
// Define enum testing support
MU_DEFINE_ENUM(MY_ENUM, MY_ENUM_VALUES)
MU_DEFINE_ENUM_STRINGS(MY_ENUM, MY_ENUM_VALUES)
CTEST_DEFINE_ENUM_TYPE(MY_ENUM, MY_ENUM_VALUES)

// Define custom type comparison
CTEST_COMPARE(my_type, MY_STRUCT)
{
    return left.field != right.field;
}

CTEST_TO_STRING(my_type, MY_STRUCT, string, bufferSize, value)
{
    snprintf(string, bufferSize, "field=%d", value.field);
}
```

### Assertion Patterns
- **Type-Safe**: `CTEST_ASSERT_ARE_EQUAL(type, expected, actual, optional_message...)`
- **Null Checks**: `CTEST_ASSERT_IS_NULL(ptr)`, `CTEST_ASSERT_IS_NOT_NULL(ptr)`
- **Boolean**: `CTEST_ASSERT_IS_TRUE(expr)`, `CTEST_ASSERT_IS_FALSE(expr)`
- **Force Fail**: `CTEST_ASSERT_FAIL(message...)`
- **Optional Messages**: All assertions support `printf`-style format strings as final arguments

### Windows-Specific Features
- **VLD Integration**: Automatic memory leak detection with `USE_VLD` compilation flag
- **Console Coloring**: ANSI color support with `USE_COLORING` option
- **Windows Types**: Built-in support for `HRESULT`, `ULONG64`, `LONG64` assertions
- **Abort on Fail**: `CTEST_ABORT_ON_FAIL` for debugging (calls `abort()` instead of longjmp)

## Build Configuration

### CMake Options
```cmake
-Drun_unittests=ON          # Enable test building
-Duse_coloring=ON|OFF       # Console output coloring (default ON for MSVC/Linux)
-Dabort_on_fail=ON|OFF      # Abort process on test failure (default OFF)
```

### Integration Pattern
```cmake
if ((NOT TARGET ctest) AND (EXISTS ${CMAKE_CURRENT_LIST_DIR}/deps/ctest/CMakeLists.txt))
    add_subdirectory(deps/ctest)
endif()

target_link_libraries(your_test_exe ctest)
```

## Test Organization Conventions

### File Structure
- One test suite per `.c` file in `tests/` subdirectories
- Test files follow pattern: `*tests.c` or `*_ut.c`
- Each test directory has its own `CMakeLists.txt`

### Test Naming
- **Test Functions**: Descriptive names using underscores (`function_with_NULL_parameter_fails`)
- **Test Suites**: Module or component names (`simpletestsuiteonetest`, `enum_define_tests`)
- **Arrange/Act/Assert**: Follow AAA pattern in test functions

### Example Test Categories
- **Unit Tests**: `tests/ctest_ut/` - Core functionality testing
- **Macro Tests**: `tests/ctest_macro_hooks_ut/` - Testing macro expansion and hooks
- **Custom Fixtures**: `tests/ctest_custom_fixtures_ut/` - Fixture system validation

## External Dependencies

### Required Submodules
- **c-build-tools**: Build infrastructure, pipeline templates, coding standards
- **c-logging**: Logging framework for test output (`LogInfo`, `LogError`, `LogWarning`)
- **macro-utils-c**: Macro metaprogramming for test function generation

### Build Infrastructure
- **Pipeline**: Uses `c-build-tools/pipeline_templates/build_all_flavors.yml`
- **Cross-Platform**: Windows (MSVC), Linux (GCC), with specialized build scripts
- **Quality Gates**: CodeQL analysis, IWYU include checking, reals checking

## Platform-Specific Notes

### Windows Features
- **ETW Integration**: Future extensibility for Windows event tracing
- **VLD Support**: Visual Leak Detector integration for memory leak detection
- **Console Enhancement**: Virtual terminal processing for colored output

### Linux/Unix Support
- **Valgrind Ready**: Compatible with memory analysis tools
- **POSIX Compliance**: Uses standard C library functions for portability

## External References
For comprehensive coding standards and build conventions, refer to:
- [c-build-tools Copilot Instructions](../deps/c-build-tools/.github/copilot-instructions.md)
- [c-logging Copilot Instructions](../deps/c-logging/.github/copilot-instructions.md)  
- [macro-utils-c Copilot Instructions](../deps/macro-utils-c/.github/copilot-instructions.md)
- [General Coding Instructions](../deps/c-build-tools/.github/general_coding_instructions.md)

---
**All code changes must strictly follow the general coding guidelines in #file:../deps/c-build-tools/.github/general_coding_instructions.md.**
