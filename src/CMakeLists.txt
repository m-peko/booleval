set (BOOLEVAL_INCLUDE_DIR ../include)

include_directories (
    BEFORE
    ${BOOLEVAL_INCLUDE_DIR}
)

set (
    SOURCE_FILES
        token/token.cpp
        token/token_type.cpp
        token/tokenizer.cpp

        tree/result_visitor.cpp
        tree/tree_node.cpp

        utils/any_value.cpp
        utils/string_utils.cpp

        evaluator.cpp
)

set (
    INCLUDE_FILES
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/token.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/token_type.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token/tokenizer.h

        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/base_visitor.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/result_visitor.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree/tree_node.h

        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/any_value.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/string_utils.h

        ${BOOLEVAL_INCLUDE_DIR}/booleval/evaluator.h
)

add_library (
    booleval ${LIBBOOLEVAL_TYPE}
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
)

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