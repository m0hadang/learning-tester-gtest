#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>

class Unit {
public:
  virtual ~Unit() {}  
  virtual void stop() = 0;
  virtual void say(const std::string& message) = 0;
  virtual void attack(Unit* target) = 0;
  virtual void move(int x, int y) = 0;
  virtual int get_y() const = 0;
};

class MockUnit : public Unit {
public:
  MOCK_METHOD0(stop, void());
  // MOCK_METHOD(void, stop, (), (override));
  MOCK_METHOD1(say, void(const std::string&));
  // MOCK_METHOD(void, say, (const std::string&), (override));
  MOCK_METHOD1(attack, void(Unit*));
  // MOCK_METHOD(void, attack, (Unit*), (override));
  MOCK_METHOD2(move, void(int, int));
  // MOCK_METHOD(void, move, (int, int), (override));
  MOCK_CONST_METHOD0(get_y, int());
  // MOCK_METHOD(int, get_y, (), (const, override));
};


// Check 1 : call or not
void foo_stop(MockUnit* p) { p->stop(); }

TEST(GMOCK_EXPECT_CALL_TEST, stop) {
  MockUnit mock;
  EXPECT_CALL(mock, stop());
  foo_stop(&mock);
}

// Check 2 : argument
void foo_say(MockUnit* p) { p->say("hello"); }

TEST(GMOCK_EXPECT_CALL_TEST, say) {
  MockUnit mock;
  EXPECT_CALL(mock, say("hello"));
  foo_say(&mock);
}

// Check 3 : how many ?
void foo_move(MockUnit* p) { 
  p->move(1, 2);
  p->move(0, 0);
  p->move(1, 2);
  // p->move(3, 0); if call this, test will be fail
}

TEST(GMOCK_EXPECT_CALL_TEST, move) {
  MockUnit mock;
  // it not exists one of EXPECT_CALL which below here 
  // then test will be fail
  EXPECT_CALL(mock, move(1, 2))
    .Times(2);
  EXPECT_CALL(mock, move(0, 0)) 
    .Times(1);

  // checking if move() is called more
  // checking if move() is called with unexpected argument(not (1, 2) and (0, 0))
  foo_move(&mock);

  // on destruct mock, checking if move() is called [(1, 2) X 2, (0, 0) X 1]
}

// Check 4 : return
int foo_get_y(MockUnit* p) { return p->get_y(); }

TEST(GMOCK_EXPECT_CALL_TEST, get_y) {
  MockUnit mock;
  EXPECT_CALL(mock, get_y())
    .Times(1)// 생략 가능
    .WillOnce(::testing::Return(3));

  int y = foo_get_y(&mock);
  EXPECT_EQ(y, 3);
}


// Check 5 : sequence
void foo_attack_and_move(MockUnit* p) { 
  p->attack(nullptr);
  p->move(10, 20);
}

TEST(GMOCK_EXPECT_CALL_TEST, attack_and_move) {
  //if you need to check sequence, add this
  ::testing::InSequence seq;

  MockUnit mock;

  EXPECT_CALL(mock, attack(nullptr));
  EXPECT_CALL(mock, move(10, 20));

  foo_attack_and_move(&mock);
}

// Any Argument
TEST(GMOCK_EXPECT_CALL_TEST, move_with_any_argument) {
  using ::testing::_;

  MockUnit mock;
  EXPECT_CALL(mock, move(_, _))// just use any argument
    .Times(3);
  // EXPECT_CALL(mock, move)// or use this
  //   .Times(3);

  mock.move(10, 20);
  mock.move(1, 5);
  mock.move(3, 6);
}

// Argument Condition
TEST(GMOCK_EXPECT_CALL_TEST, move_with_ge) {
  using ::testing::_;

  MockUnit mock;
  EXPECT_CALL(mock, move(::testing::Ge(2), _))// just use any argument
    .Times(3);

  // mock.move(1, 20);
  mock.move(4, 20);
  mock.move(2, 5);
  mock.move(3, 6);
}

// Duplicate EXPECT_CALL
TEST(GMOCK_EXPECT_CALL_TEST, duplicate_get_y) {

  return;

  //fail test

  MockUnit mock;
  EXPECT_CALL(mock, get_y()) // duplicate get_y()
    .WillOnce(::testing::Return(20));
  EXPECT_CALL(mock, get_y()) // duplicate get_y()
    .WillOnce(::testing::Return(10));// #1

  EXPECT_EQ(mock.get_y(), 10);// match #1
  EXPECT_EQ(mock.get_y(), 20);// match #1, test fail, because #1 is over(second call)
}