#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class Cat {
public:
  std::string name_;
  Cat(const std::string name) : name_(name) {}
};

class ICatBuilder {
public:
  virtual Cat create() = 0;
};

class MockCatBuilder : public ICatBuilder {
public:
  MOCK_METHOD(Cat, create, (), (override));
};

//default value is not recommended
//because default value make test hard to understance
TEST(GMOCK_DEFAULT_VALUE_TEST, create) {

  //set default cat
  Cat default_cat("navi");
  ::testing::DefaultValue<Cat>::Set(default_cat);//copy

  // don't need to specify an action here, as the default
  // return value works for us.
  MockCatBuilder mock;
  EXPECT_CALL(mock, create());

  default_cat.name_ = "ari";//update but not change default value

// This should return default_cat.
  Cat clone_cat = mock.create();
  EXPECT_EQ(clone_cat.name_, "navi");

  // Unsets the default return value.
  ::testing::DefaultValue<Cat>::Clear();
}