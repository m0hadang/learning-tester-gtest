#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

class MockCalc {
public:
  MOCK_METHOD(int, sum, (int x, int y), ());
  MOCK_METHOD(int, complex_job, (int x), ());
};

int static_sum(int x, int y) { 
  return x + y; 
}

class X_10 {
public:
  int complex_job(int x){
    return x * 10;
  }
};

TEST(GMOCK_INVOKE_TEST, complex_job) {
  using ::testing::_; 

  MockCalc mock;
  X_10 helper;

  EXPECT_CALL(mock, sum(_, _))
    .WillOnce(&static_sum);

  EXPECT_CALL(mock, complex_job(_))
    .WillOnce(::testing::Invoke(&helper, &X_10::complex_job))
    .WillOnce([] { return 1; })
    .WillRepeatedly([](int x) { return x * -1; });

  EXPECT_EQ(mock.sum(5, 6), 11);//Invokes static_sum(5, 6).
  EXPECT_EQ(mock.complex_job(10), 100);//Invokes helper.ComplexJob(10).
  EXPECT_EQ(mock.complex_job(10), 1);//Invokes the inline lambda.
  EXPECT_EQ(mock.complex_job(10), -10);//Invokes the inline lambda.
}