cmake_minimum_required(VERSION 3.12)

#
# Manually specifying all the source files.
#
set(PROJECT_NAME dynamicPriorityQueue)
# set(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(${PROJECT_NAME} CXX)

set(HEADER_FILES
    priority_queue.hpp)

set(UNIT_TEST_FILES
    TestPriorityQueue.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compiler flags for YouCompleteMe

#
# This is the main target
#
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
add_executable(UnitTestRunner  ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES})

#
# We want the C++ 20 standard for our project
#
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET UnitTestRunner PROPERTY CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
    target_compile_options(UnitTestRunner PRIVATE /W4 /permissive-)
    # set_target_properties(UnitTestRunner PROPERTIES LINK_FLAGS "/STACK:100000000")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(UnitTestRunner PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message("AppleClang Compiler")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(UnitTestRunner PRIVATE -Wall -Wextra -pedantic)
endif()

#
# Prepare a pre-build step to run clang-format over all the [ch]pp source files.
# Start by finding the location of the clang-format executable.
#
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    #
    # Need to take the simple source file locations used for the project and get their full
    # file system locations for use in putting together the clang-format command line
    #
    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES} main.cpp)
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()

    #
    # This creates the clang-format target/command
    #
    add_custom_target(
        ClangFormat
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${SOURCE_FILES_PATHS}
    )
    #
    # This makes the clang-format target a dependency of the main GoogleTestIntro project
    #
    add_dependencies(${PROJECT_NAME} ClangFormat)
else()
    message("Unable to find clang-format")
endif()

#
# Add GoogleTest
#
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

## Prevent overriding the parent project's compiler/linker
## settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

## Add googletest directly to our build. This defines
## the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(UnitTestRunner gtest_main)
