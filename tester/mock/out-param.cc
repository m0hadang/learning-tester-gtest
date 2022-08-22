#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class INetDriver {
public:
  virtual void get_error(int& err) const = 0;
  virtual bool get_spec(std::vector<int>& v, int& id) = 0;
};

class MockNetDriver : public INetDriver {
public:
  MOCK_CONST_METHOD1(get_error, void(int&));
  MOCK_METHOD2(get_spec, bool(std::vector<int>&, int&));
};

// Set Action : single out parameter
TEST(GMOCK_OUT_PARAM_TEST, get_error) {
  using ::testing::_;
  MockNetDriver mock;

  int i = 10;
  EXPECT_CALL(mock, get_error(_))
    .WillOnce(::testing::SetArgReferee<0>(i));

  int out_i;
  mock.get_error(out_i);
  EXPECT_EQ(out_i, 10);
  EXPECT_EQ(out_i, i);
  EXPECT_NE(&out_i, &i);
}

// Set Action : multiple out parameteres
// can set multiple action with DoAll
TEST(GMOCK_OUT_PARAM_TEST, get_spec) {
  using ::testing::_;
  MockNetDriver mock;

  std::vector<int> v{0,1,2};
  int i = 10;
  EXPECT_CALL(mock, get_spec(_, _))
    .WillOnce(
      ::testing::DoAll(
        ::testing::SetArgReferee<0>(v), 
        ::testing::SetArgReferee<1>(i), 
        ::testing::Return(true)));

  std::vector<int> out_v;
  int out_i;
  mock.get_spec(out_v, out_i);

  EXPECT_EQ(out_i, 10);
  EXPECT_EQ(out_i, i);
  EXPECT_EQ(out_v[0], 0);
  EXPECT_EQ(out_v[1], 1);
  EXPECT_EQ(out_v[2], 2);
}