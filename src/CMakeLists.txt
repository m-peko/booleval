set (BOOLEVAL_INCLUDE_DIR ../include)

include_directories (
    BEFORE
    ${BOOLEVAL_INCLUDE_DIR}
)

set (
    SOURCE_FILES
        token/tokenizer.cpp
        tree/expression_tree.cpp
)

set (
    INCLUDE_FILES
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/token.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/token_type.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/tokenizer.hpp

        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/expression_tree.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/result_visitor.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/tree_node.hpp

        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/any_mem_fn.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/any_value.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/split_range.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/string_utils.hpp

        ${BOOLEVAL_INCLUDE_DIR}/booleval/evaluator.hpp
        ${BOOLEVAL_INCLUDE_DIR}/booleval/exceptions.hpp
)

add_library (
    booleval ${LIBBOOLEVAL_TYPE}
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
)

if (NOT MSVC)
    target_link_libraries (booleval --coverage)
endif()

set_target_properties (booleval PROPERTIES OUTPUT_NAME booleval)
set_target_properties (booleval PROPERTIES VERSION ${LIBBOOLEVAL_VERSION} SOVERSION ${LIBBOOLEVAL_VERSION} )

# Install instructions for this target
install (
    TARGETS booleval
    EXPORT libboolevalTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT dev
)

macro (install_headers_with_directory header_list)
    foreach (header ${header_list})
        # Extract directory name and remove leading '../'
        get_filename_component (dir ${header} PATH)
        string (REGEX REPLACE "^\\.\\.\\/" "" DIR ${dir})
        install (FILES ${header} DESTINATION ${dir})
    endforeach (header)
endmacro ()

install_headers_with_directory (${INCLUDE_FILES})