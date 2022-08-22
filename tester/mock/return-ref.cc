#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class Car {
public:
  int data_;
};

class CarBuilder {
public:
  virtual ~CarBuilder() {}  
  virtual Car get_copy_bar() = 0;
  virtual Car& get_ref_bar() = 0;
  virtual int get_copy_value() = 0;
};

class MockCarBuilder : public CarBuilder {
public:
  MOCK_METHOD(Car, get_copy_bar, (), (override));
  MOCK_METHOD(Car&, get_ref_bar, (), (override));
  MOCK_METHOD(int, get_copy_value, (), (override));
};

TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, wrong_get_copy_bar) {
  MockCarBuilder car_builder;
  Car car1;
  car1.data_ = 10;
  EXPECT_CALL(car_builder, get_copy_bar())
    .WillRepeatedly(::testing::Return(car1));//copied car1 data
    
  car1.data_ = 20;//change value 10 -> 20
  Car bar2 = car_builder.get_copy_bar();//but alwayse return car1(10)

  EXPECT_EQ(bar2.data_, 10);//still value is 10
}


// if you want to return refrence value 
// then use 'ReturnRef'
TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, get_ref_bar) {
  MockCarBuilder car_builder;
  Car car1;
  car1.data_ = 10;
  EXPECT_CALL(car_builder, get_ref_bar())
    .WillRepeatedly(::testing::ReturnRef(car1));
  
  car1.data_ = 20;
  Car& bar2 = car_builder.get_ref_bar();
  
  EXPECT_EQ(car1.data_, 20);
  EXPECT_EQ(car1.data_, bar2.data_);
  EXPECT_EQ(&car1, &bar2);
}

// if you use 'ReturnPointee' then 
// you can use get_copy_bar
TEST(GMOCK_EXPECT_CALL_RETURN_REF_TEST, get_copy_bar) {
  MockCarBuilder car_builder;
  Car car1;
  car1.data_ = 10;
  EXPECT_CALL(car_builder, get_copy_bar())
    .WillRepeatedly(::testing::ReturnPointee(&car1));
  
  car1.data_ = 20;
  Car bar2 = car_builder.get_copy_bar();//use copy bar but working
  
  EXPECT_EQ(car1.data_, 20);
  EXPECT_EQ(car1.data_, bar2.data_);
  EXPECT_NE(&car1, &bar2);// not same instance
}