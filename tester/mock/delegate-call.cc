#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class IShark {
public:
  virtual ~IShark() {}
  virtual char do_this(int n) = 0;
  virtual void do_that(const char* s, int* p) = 0;
};

class FakeShark : public IShark {
public:
  char do_this(int n) override {
    return (n > 0) ? '+' :
          (n < 0) ? '-' : '0';
  }
  void do_that(const char* s, int* p) override {
    *p = strlen(s);
  }  
};

class MockShark : public IShark {
public:
  // normal mock method definitions using gmock.
  MOCK_METHOD(char, do_this, (int n), (override));
  MOCK_METHOD(void, do_that, (const char* s, int* p), (override));

  void delegate_to_fake() { // should call this before call another ON_CALL
    ON_CALL(*this, do_this).WillByDefault([this](int n) {
      return fake_.do_this(n);
    });
    ON_CALL(*this, do_that).WillByDefault([this](const char* s, int* p) {
      fake_.do_that(s, p);
    });
  }

private:
  FakeShark fake_;  // keeps an instance of the fake in the mock.
  //RedShark real_; // can use product object instead fake
};

TEST(GMOCK_DELEGATE_CALL_TEST, shark) {
  MockShark mock;

  mock.delegate_to_fake();  // active fake

  // Put your ON_CALL(mock, ...)s here, if any override
  // ON_CALL(mock, do_this).WillByDefault([this](int n) {
  //   return '-';
  // });

  // if not exist define abount action, then call default action
  EXPECT_CALL(mock, do_this(5));
  EXPECT_CALL(mock, do_that(::testing::_, ::testing::_));

  int n = 0;
  EXPECT_EQ('+', mock.do_this(5));  // FakeShark::do_this() is invoked.
  mock.do_that("Hi", &n);  // FakeShark::do_that() is invoked.
  EXPECT_EQ(2, n);
}