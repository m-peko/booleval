cmake_minimum_required (VERSION 3.2)

project (booleval)

option (BOOLEVAL_BUILD_TESTS "Build tests" ON)

set (CMAKE_STATIC_LIBRARY_PREFIX "")
set (CMAKE_C_FLAGS "-O3 -DSHBRELEASE -ggdb3 -Wall -fPIC")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions (-Wl,-h$@)

set (BOOLEVAL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (BOOLEVAL_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories (${BOOLEVAL_INCLUDE_DIR})

include (ExternalProject)

set (
    INCLUDE_FILES
        ${BOOLEVAL_INCLUDE_DIR}/base_token.h
        ${BOOLEVAL_INCLUDE_DIR}/evaluator.h
        ${BOOLEVAL_INCLUDE_DIR}/object.h
        ${BOOLEVAL_INCLUDE_DIR}/token.h
        ${BOOLEVAL_INCLUDE_DIR}/tokenizer.h
        ${BOOLEVAL_INCLUDE_DIR}/tree_node.h
)

set (
    SOURCE_FILES
        ${BOOLEVAL_SOURCE_DIR}/base_token.cpp
        ${BOOLEVAL_SOURCE_DIR}/evaluator.cpp
        ${BOOLEVAL_SOURCE_DIR}/object.cpp
        ${BOOLEVAL_SOURCE_DIR}/tokenizer.cpp
        ${BOOLEVAL_SOURCE_DIR}/tree_node.cpp
)

add_library (
    libbooleval
    ${SOURCE_FILES}
)

if (BOOLEVAL_BUILD_TESTS)
    # Only include googletest if the git submodule has been fetched
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
        # Enable tests and add the test directory
        message (STATUS "Tests have been enabled")
        set (GOOGLETEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
        set (GOOGLETEST_INCLUDE ${GOOGLETEST_ROOT}/googletest/include)
        set (GOOGLETEST_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest)
        set (GOOGLETEST_LIBRARY ${GOOGLETEST_BINARY_DIR}/googletest)

        ExternalProject_Add (
            googletest
            DOWNLOAD_COMMAND ""
            SOURCE_DIR ${GOOGLETEST_ROOT}
            BINARY_DIR ${GOOGLETEST_BINARY_DIR}
            CMAKE_CACHE_ARGS "-DBUILD_GTEST:bool=ON"
                             "-DBUILD_GMOCK:bool=OFF"
                             "-Dgtest_force_shared_crt:bool=ON"
                             "-DCMAKE_CXX_COMPILER:path=${CMAKE_CXX_COMPILER}"
            INSTALL_COMMAND ""
        )
        # Make sure we build googletest before anything else
        add_dependencies (libbooleval googletest)
        enable_testing ()
        add_subdirectory (googletest)
        add_subdirectory (tests)
    else ()
        message (STATUS "googletest git submodule is absent. Run `git submodule init && git submodule update` to get it")
    endif ()
endif ()

add_executable (
    booleval
    main.cpp
    ${SOURCE_FILES}
    ${INCLUDE_FILES}
)
