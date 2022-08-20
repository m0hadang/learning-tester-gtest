#include <gtest/gtest.h>

struct account_state {
  int test_data;
  int answer;
};

struct ParameterTest : 
  public 
    ::testing::Test, 
    testing::WithParamInterface<account_state> {
};

TEST_P(ParameterTest, mul) {
  int y = GetParam().test_data;
  EXPECT_EQ(
    10 * y, GetParam().answer);
}

INSTANTIATE_TEST_SUITE_P(
  Default, ParameterTest,
  testing::Values(
    account_state{4, 40},
    account_state{3, 30}));