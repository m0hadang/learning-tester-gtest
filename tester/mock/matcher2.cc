#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

class SomeStruct {
public:
  // field
  int data1_;
  std::string data2_;
public:
  // property : no arg, const member function
  std::string data2() const {
    return data2_;
  }  
};

class MockRadio {
public:
  MOCK_METHOD(void, some_method, (SomeStruct), ());
};

// field
TEST(GMOCK_MEMBER_DATA_TEST, field) {

  MockRadio some_mock;
  EXPECT_CALL(
    some_mock, 
    some_method(::testing::Field(&SomeStruct::data1_, 19)));

  SomeStruct  some_struct;
  some_struct.data1_ = 19;
  some_mock.some_method(some_struct);
}

// property
TEST(GMOCK_MEMBER_DATA_TEST, property) {

  MockRadio some_mock;
  EXPECT_CALL(
    some_mock, 
    some_method(
      ::testing::Property(
        &SomeStruct::data2, ::testing::StartsWith("John ")
      )
    )
  );

  SomeStruct  some_struct;
  some_struct.data2_ = "John Whick";
  some_mock.some_method(some_struct);
}

// combine
TEST(GMOCK_MEMBER_DATA_TEST, combine) {

  ::testing::Matcher<SomeStruct> matcher = 
    ::testing::AllOf(
      ::testing::Field(&SomeStruct::data1_, 19),
      ::testing::Property(&SomeStruct::data2, ::testing::StartsWith("John "))
    );

  MockRadio some_mock;
  EXPECT_CALL(some_mock, some_method(matcher));

  SomeStruct  some_struct;
  some_struct.data1_ = 19;
  some_struct.data2_ = "John Whick";
  some_mock.some_method(some_struct);
}