cmake_minimum_required(VERSION 3.10)

project(log)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(BUILD_EXAMPLES)
  include(cmake/examples.cmake)
endif()

include(CTest)
if(BUILD_TESTING)
   include(cmake/tests.cmake)
endif()

