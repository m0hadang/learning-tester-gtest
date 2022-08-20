#include <gtest/gtest.h>

#include <iostream>

class IOException : public std::exception {
public:
  virtual const char* what() { return "Pipe is broken"; }
};

struct IConnection {
  virtual void move(int x, int y) = 0;
  virtual void attack() = 0;
  virtual ~IConnection() {}
};

// real object
class TCPConnection : public IConnection {
  void move(int x, int y) override {
    std::cout << "do move" << std::endl;
  }
  void attack() override {
    std::cout << "do attack" << std::endl;
  }
};

// stub object(test double)
class StubBadConnection : public IConnection { 
public:
  virtual void move(int x, int y) override { 
    throw IOException();
  }
  virtual void attack() override { 
    throw IOException();
  }
};

// test target
class User { 
  IConnection* connection;
public:
  User(IConnection* p) : connection(p) {}
  void MoveUser(int x, int y) { connection->move(x, y); }
  bool AttackUser() { 
    try {
      connection->attack(); 
    } catch(const std::exception& e) {
      return false;
    }
    return true;
  }
};

class UserTest : public ::testing::Test {
protected:
  User* user;
  virtual void SetUp() override { user = new User(new StubBadConnection); }

  virtual void TearDown() override { delete user; }
};

TEST_F(UserTest, move) {
  ASSERT_THROW(user->MoveUser(10, 10), IOException);
}

TEST_F(UserTest, attack) {
  ASSERT_EQ(user->AttackUser(), false);
}