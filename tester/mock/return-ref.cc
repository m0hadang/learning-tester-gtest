#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class Bar {
public:
  int data_;
};

class Foo {
public:
  virtual ~Foo() {}  
  virtual Bar get_copy_bar() = 0;
  virtual Bar& get_ref_bar() = 0;
  virtual int get_copy_value() = 0;
};

class MockFoo : public Foo {
public:
    MOCK_METHOD(Bar, get_copy_bar, (), (override));
    MOCK_METHOD(Bar&, get_ref_bar, (), (override));
    MOCK_METHOD(int, get_copy_value, (), (override));
};

TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, wrong_get_copy_bar) {
  MockFoo foo;
  Bar bar1;
  bar1.data_ = 10;
  EXPECT_CALL(foo, get_copy_bar())
    .WillRepeatedly(::testing::Return(bar1));//copied bar1 data
    
  bar1.data_ = 20;//change value 10 -> 20
  Bar bar2 = foo.get_copy_bar();//but alwayse return bar1(10)

  EXPECT_EQ(bar2.data_, 10);//still value is 10
}


// if you want to return refrence value 
// then use 'ReturnRef'
TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, get_ref_bar) {
  MockFoo foo;
  Bar bar1;
  bar1.data_ = 10;
  EXPECT_CALL(foo, get_ref_bar())
    .WillRepeatedly(::testing::ReturnRef(bar1));
  
  bar1.data_ = 20;
  Bar& bar2 = foo.get_ref_bar();
  
  EXPECT_EQ(bar2.data_, 20);
  EXPECT_EQ(bar1.data_, bar2.data_);
  EXPECT_EQ(&bar1, &bar2);
}

// if you use 'ReturnPointee' then 
// you can use get_copy_bar
TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, get_copy_bar) {
  MockFoo foo;
  Bar bar1;
  bar1.data_ = 10;
  EXPECT_CALL(foo, get_copy_bar())
    .WillRepeatedly(::testing::ReturnPointee(&bar1));
  
  bar1.data_ = 20;
  Bar bar2 = foo.get_copy_bar();//use copy bar but working
  
  EXPECT_EQ(bar2.data_, 20);
  EXPECT_EQ(bar1.data_, bar2.data_);
  EXPECT_NE(&bar1, &bar2);// not same instance
}