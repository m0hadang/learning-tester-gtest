#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <not-implemented-exception.hh>
#include <iostream>
#include <string>

class Time {
public:
  std::string get_time_string() { 
    throw NotImplementedException();
  }
};

class MockTime : public Time {
public:
  MOCK_METHOD(std::string, get_time_string, ());
};

using ::testing::Return;
TEST(GMOCK_ON_CALL_TEST, get_time_string) {
  MockTime stub;
  ON_CALL(stub, get_time_string())// if stub call get_time_string
    .WillByDefault(Return("00:00"));//then will return "00:00"
  EXPECT_EQ(stub.get_time_string(), "00:00");
}