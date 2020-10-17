cmake_minimum_required (VERSION 3.2)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_custom_target (
    examples DEPENDS
    evaluator
)

add_executable (evaluator EXCLUDE_FROM_ALL evaluator.cpp)
