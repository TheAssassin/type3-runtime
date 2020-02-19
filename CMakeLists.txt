# required for hassle-free C11/C++11 support
cmake_minimum_required(VERSION 3.1)

project(appimage-type3-runtime C CXX)

# we use C11 for the runtime implementation
set(CMAKE_C_STANRARD 11)
set(CMAKE_C_STANRARD_REQUIRED ON)

# we use C++11 for the tests
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include external dependencies configuration
include(cmake/tools.cmake)
include(cmake/dependencies.cmake)

# build static binary
message(STATUS "Setting up static builds")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static -no-pie")

add_subdirectory(src)

add_subdirectory(tests)
