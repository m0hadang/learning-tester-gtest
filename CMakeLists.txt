cmake_minimum_required(VERSION 3.2)
set(This learning-tester-gtest)
project(${This} C CXX)

cmake_policy(SET CMP0091 NEW) # msvc, MSVC_RUNTIME_LIBRARY, googletest Md

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

enable_testing()
add_subdirectory(googletest)
add_subdirectory(test)