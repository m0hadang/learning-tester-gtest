#include <gtest/gtest.h>

struct FixtureTest : public ::testing::Test {
  int* x;

  int GetX() { return *x; }
  
  static void SetUpTestCase() {
    // it is called once
    std::cout << "# SetUpTestCase" << std::endl;
  }

  static void TearDownTestCase() {
    // it is called once
    std::cout << "# TearDownTestCase" << std::endl;
  }

  FixtureTest() {
    // it is called in every TEST_F
    std::cout << "## FixtureTest" << std::endl;
  }

  virtual void SetUp() override {
    // it is called in every TEST_F
    // prefare for test
    std::cout << "### SetUp" << std::endl;
    x = new int(10);
  }

  virtual void TearDown() override {
    // it is called in every TEST_F
    // clear test leftover
    std::cout << "### TearDown" << std::endl;
    delete x;
  }  
};

TEST_F(FixtureTest, add) { 
  //create new FixtureTest object
  int x = GetX();
  EXPECT_EQ(30, x + 20);
}

TEST_F(FixtureTest, add2) {
  //create new FixtureTest object
  int x = GetX();
  EXPECT_EQ(-10, x - 20);
}