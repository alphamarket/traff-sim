cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-D__TEST__")

project(GA_TSP_TEST)

file(GLOB_RECURSE TEST_SOURCES)
file(GLOB_RECURSE TEST_HEADERS "hpp/*.hpp")

set (TEST_INCLUDE_DIRS "")
foreach (_headerFile ${TEST_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND TEST_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES TEST_INCLUDE_DIRS)

include_directories(${TEST_INCLUDE_DIRS})
add_executable (test testerMain.cpp ${TEST_SOURCES})

# set warnings level
IF(MSVC) # MSVC compiler (Win32 only)
    # Display more warnings
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSEIF(UNIX OR CMAKE_COMPILER_IS_GNUCXX) # Clang OR Gcc (Linux, Mac OS or Win32 with MingW)
    # displays all warnings
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
ENDIF()