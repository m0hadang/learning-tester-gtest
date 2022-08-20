cmake_minimum_required(VERSION 3.2)

set(This Tester)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)

include_directories (
  ../include
)

set(Sources
  simple-test.cc
  throw-test.cc
  fixture-test.cc
  parameter-test.cc
  timeout-test.cc
  double/stub.cc
  double/dumy.cc
  double/fake.cc
  double/spy.cc
)

add_executable(${This} ${Sources})

target_link_libraries(${This} PUBLIC
  gtest_main
  gmock_main
)

add_test(
  NAME ${This}
  COMMAND ${This}
)