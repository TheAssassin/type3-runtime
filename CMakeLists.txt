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

# make the compiler complain about every warning
message(STATUS "Setting up paranoia mode")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_subdirectory(lib)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
