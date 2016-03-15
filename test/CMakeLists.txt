cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

project(LIZARD_TESTER)

file(GLOB_RECURSE iSOURCES "../src/*.cpp")
file(GLOB_RECURSE iHEADERS "lizard/*.hpp")

set (iINCLUDE_DIRS "")
foreach (_headerFile ${iHEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND iINCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES iINCLUDE_DIRS)

include_directories(${iINCLUDE_DIRS} ".." ".")

add_executable (test lizard/lizard.cpp ${iSOURCES})

target_link_libraries (test pthread)

# set warnings level
IF(MSVC) # MSVC compiler (Win32 only)
    # Display more warnings
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
ELSEIF(UNIX OR CMAKE_COMPILER_IS_GNUCXX) # Clang OR Gcc (Linux, Mac OS or Win32 with MingW)
    # displays all warnings
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
ENDIF()