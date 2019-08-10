set (BOOLEVAL_INCLUDE_DIR ../include)

include_directories (
    BEFORE
    ${BOOLEVAL_INCLUDE_DIR}
)

set (
    SOURCE_FILES
        nodes/and_node.cpp
        nodes/eq_node.cpp
        nodes/gt_node.cpp
        nodes/lt_node.cpp
        nodes/neq_node.cpp
        nodes/or_node.cpp

        utils/regex_pattern.cpp
        utils/string_utils.cpp

        #evaluator.cpp
        token.cpp
        tokenizer.cpp
)

set (
    INCLUDE_FILES
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/and_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/base_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/eq_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/gt_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/lt_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/neq_node.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/nodes/or_node.h

        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/regex_pattern.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/utils/string_utils.h

        #${BOOLEVAL_INCLUDE_DIR}/booleval/evaluator.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tokenizer.h
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