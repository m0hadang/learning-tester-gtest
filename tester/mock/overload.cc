#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

class IOverLoad {
public:  
  virtual int sum(int a, int b) = 0;
  virtual int sum(int a, int b, int c) = 0;
  virtual int not_overload(int a, int b, int c) = 0;
};

class MockOverLoad : public IOverLoad {
public:
  MOCK_METHOD(int, sum, (int a, int b), (override));
  MOCK_METHOD(int, sum, (int a, int b, int c), (override));
  MOCK_METHOD(int, not_overload, (int a, int b, int c), (override));
};

TEST(GMOCK_OVERLOAD_TEST, sum) {
  MockOverLoad mock;

  EXPECT_CALL(mock, sum(1, 4))
    .WillOnce(::testing::Return(5));
  EXPECT_CALL(mock, sum(1, 4, 5))
    .WillOnce(::testing::Return(10));
  // EXPECT_CALL(mock, sum)           // error, which call ?
  //   .WillOnce(::testing::Return(10));
  EXPECT_CALL(mock, not_overload)     // not overload so not error
    .WillOnce(::testing::Return(123));

  EXPECT_EQ(mock.sum(1, 4), 5);
  EXPECT_EQ(mock.sum(1, 4, 5), 10);
  EXPECT_EQ(mock.not_overload(1, 2, 3), 123);
}