#include <gtest/gtest.h>
#include <not-implemented-exception.hh>
#include <iostream>

class IOException : public std::exception {
public:
  virtual const char* what() { return "pipe is broken"; }
};

struct IConnection {
  virtual void move(int x, int y) = 0;
  virtual void attack() = 0;
  virtual ~IConnection() {}
};

// real object
class TCPConnection : public IConnection {
  void move(int x, int y) override {
    throw NotImplementedException();
  }
  void attack() override {
    throw NotImplementedException();
  }
};

/*
 * stub(test double)
 *
 * just return prefared value for test
 */
class StubBadConnection : public IConnection { 
public:
  virtual void move(int x, int y) override { 
    throw IOException();//prefared value for test
  }
  virtual void attack() override { 
    throw IOException();//prefared value for test
  }
};

// test target
class User { 
  IConnection* connection_;
public:
  User(IConnection* connection) 
    : connection_(connection) {}
  void move_user(int x, int y) { connection_->move(x, y); }
  bool attack_user() { 
    try {
      connection_->attack(); 
    } catch(const std::exception& e) {
      return false;
    }
    return true;
  }
};

class StubTest : public ::testing::Test {
protected:
  User* user_;
  virtual void SetUp() override { 
    user_ = new User(new StubBadConnection()); 
  }
  virtual void TearDown() override { 
    delete user_; 
  }
};

TEST_F(StubTest, move) {
  ASSERT_THROW(user_->move_user(10, 10), IOException);
}

TEST_F(StubTest, attack) {
  ASSERT_EQ(user_->attack_user(), false);
}