#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class MockPipe {
public:
  MOCK_METHOD(void, expect_call, (), ());
  MOCK_METHOD(void, unexpected_call, (), ());
};

TEST(GMOCK_UNEXPECTED_CALL_TEST, unexpected_call) {
  MockPipe mock;
  EXPECT_CALL(mock, unexpected_call)
    .Times(0);
  mock.expect_call();
  // mock.unexpected_call();//if call this then fail test
}