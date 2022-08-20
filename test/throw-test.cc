#include <gtest/gtest.h>

class MyException : public std::exception {
public:
  virtual const char* what() { return "my exception"; }
};

void RaiseMyException(bool do_raise) {
  if (do_raise) { throw MyException(); }
}

TEST(THROW_TEST, throw) {
  ASSERT_THROW(RaiseMyException(true), MyException);
}