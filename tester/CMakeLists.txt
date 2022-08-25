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
  mock/on-call.cc
  mock/expect-call.cc
  mock/return-ref.cc
  mock/out-param.cc
  mock/set-default-value.cc
  mock/invoke.cc
  mock/protect-unexpected-call.cc
  mock/overload.cc
)

add_executable(${This} ${Sources})

# if(MSVC)
#   set_property(TARGET ${This} PROPERTY
#               MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# endif()

target_link_libraries(${This} PUBLIC
  gtest_main
  gmock_main
)

add_test(
  NAME ${This}
  COMMAND ${This}
)