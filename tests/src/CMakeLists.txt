cmake_minimum_required (VERSION 3.2)

# Use booleval's include directories + test include directories
include_directories (
    ${PROJECT_SOURCE_DIR}/include/
    ${GOOGLETEST_INCLUDE}
)

# Find pthread library
find_package (Threads REQUIRED)

link_directories (
    ${GOOGLETEST_LIBRARY}
)

# Link against GoogleTest, libbooleval and pthread.
# Pthread is required by GoogleTest
link_libraries (
    gtest
    gtest_main
    booleval
)

add_custom_target (tests)

macro (create_test test_name)
    string (REPLACE "/" "_" binary_name ${test_name})
    set (binary_name "${binary_name}_test")
    add_executable (${binary_name} EXCLUDE_FROM_ALL "${test_name}_test.cpp")
    add_test (${test_name} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${binary_name})
    add_dependencies (tests ${binary_name})
    target_link_libraries (${binary_name} gtest gtest_main)
endmacro ()

# Tests

create_test (token/token)
create_test (token/tokenizer)
create_test (tree/tree_node)
create_test (utils/any_value)
create_test (utils/string_utils)
create_test (evaluator)