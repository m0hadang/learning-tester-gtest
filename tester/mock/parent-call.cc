#include <gtest/gtest.h>
#include <gmock/gmock.h>

class BaseLine {
public:
  virtual ~BaseLine() { }
  virtual void pure(int n)  = 0;
  virtual int concrete(const char* str) {
    return atoi(str);
  }
};

class MockLine : public BaseLine {
public:
  // Mocking a pure method.
  MOCK_METHOD(void, pure, (int n), (override));

  // Mocking a concrete method.
  // want to call BaseLine::concrete() but it is shadowed
  // and also want to make mock concrete
  MOCK_METHOD(int, concrete, (const char* str), (override));
};

TEST(GMOCK_PARENT_TEST, concrete) {
  MockLine mock;
  EXPECT_CALL(mock, concrete).WillOnce([&mock](const char* str) {
    // return mock.concrete(str);// not working, mock concrete is not defined
    
    return mock.BaseLine::concrete(str);// call parent class concrete
  });
  
  EXPECT_EQ(mock.concrete("12"), 12);
}