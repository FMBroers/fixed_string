cmake_minimum_required(VERSION 2.6)
 if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O0" )
set(CMAKE_VERBOSE_MAKEFILE on)
endif()
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests main.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread )
