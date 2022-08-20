#include <gtest/gtest.h>

TEST(HELLO_GOOGLETEST, HELLO_GOOGLETEST) {
  std::cout << "---------START TEST---------" << std::endl;
  EXPECT_TRUE(true);
  EXPECT_FALSE(false);
}

TEST(EXPECT_TEST, native_type) {
  EXPECT_EQ(true, true);
  EXPECT_EQ(1, 1);
  // ASSERT_EQ(0.7, 0.1 * 7); not working
  EXPECT_DOUBLE_EQ(0.7, 0.1 * 7);
  EXPECT_NEAR(0.7, 0.1 * 7, 0.01);//error range : 0.01  
}

TEST(EXPECT_TEST, string_type) {
  EXPECT_EQ("abc", "abc");
  EXPECT_STREQ("abc", "abc");
  EXPECT_EQ(std::string("abc"), "abc");
  // EXPECT_STREQ(std::string("abc"), "abc"); not working
}

TEST(EXPECT_TEST, fail_message) {
  EXPECT_EQ(2, 2) << "________if fail then show this message_______";
}

TEST(ASSERT_TEST, assert) {
  // if chnage keyword from EXPECT~ to ASSERT then stop
  // ASSERT_TRUE(false); stop here
  ASSERT_EQ("abc", "abc") << "s1 == s2";
  ASSERT_STREQ("abc", "abc");
}