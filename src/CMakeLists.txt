set (BOOLEVAL_INCLUDE_DIR ../include)

include_directories (
    BEFORE
    ${BOOLEVAL_INCLUDE_DIR}
)

set (
    SOURCE_FILES
        base_token.cpp
        evaluator.cpp
        object.cpp
        tokenizer.cpp
        tree_node.cpp
)

set (
    INCLUDE_FILES
        ${BOOLEVAL_INCLUDE_DIR}/booleval/base_token.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/evaluator.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/object.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/token.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tokenizer.h
        ${BOOLEVAL_INCLUDE_DIR}/booleval/tree_node.h
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