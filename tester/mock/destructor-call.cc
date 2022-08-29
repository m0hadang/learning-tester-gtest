#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockDestructor {
public:
  // MOCK_METHOD(void, ~MockFoo, ());  // Won't compile!

  MOCK_METHOD(void, die, ());
  // so destructor call die
  ~MockDestructor() { die(); }
};

TEST(GMOCK_DESTRUCTOR_TEST, die) {
  MockDestructor mock;
  EXPECT_CALL(mock, die);
}