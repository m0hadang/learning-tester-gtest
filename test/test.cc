#include <gtest/gtest.h>

TEST(HelloGoogleTest, DemonstrateGTestMacros) {
  EXPECT_TRUE(true);
  // ASSERT_TRUE(false);//if fail then stop here
  EXPECT_EQ(true, true);
  int result = 4;
  EXPECT_EQ(4, result) << "________if fail show this message_______";
}