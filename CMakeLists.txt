cmake_minimum_required (VERSION 3.2)

project (booleval)

option (BOOLEVAL_BUILD_EXAMPLES "Build examples" ON)
option (BOOLEVAL_BUILD_TESTS "Build tests" ON)

# Compile in release mode by default
if (NOT CMAKE_BUILD_TYPE)
    message (STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set (CMAKE_BUILD_TYPE RelWithDebInfo)
else (NOT CMAKE_BUILD_TYPE)
    message (STATUS "Using specified '${CMAKE_BUILD_TYPE}' build type.")
endif (NOT CMAKE_BUILD_TYPE)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# Compilation flags
if (MSVC)
    # Don't always use Wall, since VC's /Wall is too verbose.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /O2")
else ()
    option (BOOLEVAL_ENABLE_COVERAGE "Enable code coverage reporting" OFF)
    if (BOOLEVAL_ENABLE_COVERAGE)
        message (STATUS "Code coverage reporting is enabled.")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -O0 --coverage")
    else ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -O3")
    endif ()
endif ()

# Build output checks
option (LIBBOOLEVAL_BUILD_SHARED "Build libbooleval as a shared library." ON)
if (LIBBOOLEVAL_BUILD_SHARED)
    message (
        STATUS
        "Build will generate a shared library. "
        "Use LIBBOOLEVAL_BUILD_SHARED=0 to perform a static build"
    )
    set (LIBBOOLEVAL_TYPE SHARED)

    # Automatically create a .def file with all symbols found
    # in the input .obj files for a SHARED library on Windows.
    set (CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
else (LIBBOOLEVAL_BUILD_SHARED)
    message (STATUS "Build will generate a static library.")
    set (LIBBOOLEVAL_TYPE STATIC)
    add_definitions ("-DBOOLEVAL_STATIC=1")
endif (LIBBOOLEVAL_BUILD_SHARED)

# The version number
set (BOOLEVAL_VERSION_MAJOR 1)
set (BOOLEVAL_VERSION_MINOR 1)
set (BOOLEVAL_VERSION_PATCH 0)
set (LIBBOOLEVAL_VERSION "${BOOLEVAL_VERSION_MAJOR}.${BOOLEVAL_VERSION_MINOR}")

include (ExternalProject)

if (NOT CMAKE_INSTALL_LIBDIR)
    set (CMAKE_INSTALL_LIBDIR lib)
endif ()

##########################
### Add subdirectories ###
##########################

add_subdirectory (src)

if (BOOLEVAL_BUILD_EXAMPLES)
    message (STATUS "Examples have been enabled")
    add_subdirectory (examples)
endif ()

if (BOOLEVAL_BUILD_TESTS)
    # Only include googletest if the git submodule has been fetched
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
        # Enable tests and add the test directory
        message (STATUS "Tests have been enabled")
        set (GOOGLETEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/googletest)
        set (GOOGLETEST_INCLUDE ${GOOGLETEST_ROOT}/googletest/include)
        set (GOOGLETEST_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/googletest)
        set (GOOGLETEST_LIBRARY ${GOOGLETEST_BINARY_DIR}/googletest)

        if (${LIBBOOLEVAL_TYPE} MATCHES SHARED)
            set (gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        endif (${LIBBOOLEVAL_TYPE} MATCHES SHARED)

        enable_testing ()
        add_subdirectory (googletest)
        add_subdirectory (tests)
    else ()
        message (STATUS "googletest git submodule is absent. Run `git submodule init && git submodule update` to get it")
    endif ()
endif ()
