#include <gtest/gtest.h>
#include <not-implemented-exception.hh>
#include <iostream>
#include <unordered_map>

struct IUserDAO {
  virtual bool add_user(int id, const std::string& name) = 0;
  virtual std::string user_name(int id) = 0;
};

// real object
class MySqlDAO : public IUserDAO {
public:
  bool add_user(int id, const std::string& name) override {
    throw NotImplementedException();
  }
  std::string user_name(int id) override {
    throw NotImplementedException();
  }
};

/*
 * fake(test double)
 * 
 * imitate reality object so
 * make test target dosen't have to system depndency
 */
class FakeDAO : public IUserDAO { 
private:
  std::unordered_map<int, std::string> mem_db_;
public:
  bool add_user(int id, const std::string& name) override {
    // have simple logic
    mem_db_[id] = name;
    return true;
  }
  std::string user_name(int id) override {
    return mem_db_[id];
  }
};

// test target
class UsersManager { 
  IUserDAO* user_dao_;
  int user_count_;
public:
  UsersManager(IUserDAO* user_dao)
    : user_dao_(user_dao), user_count_(0) {}
  int add_red_user(std::string name) { 
    int id = user_count_;
    std::string new_name = "red_" + name;
    if (user_dao_->add_user(user_count_, new_name) == false){
      return -1;
    }
    ++user_count_;
    return id;
  }
  std::string red_user_name(int id) {
    return user_dao_->user_name(id);
  }
};

class FakeTest : public ::testing::Test {
protected:
  UsersManager* users_manager_;
  virtual void SetUp() override { 
    users_manager_ = new UsersManager(new FakeDAO()); 
  }

  virtual void TearDown() override { 
    delete users_manager_; 
  }
};

TEST_F(FakeTest, red_user_name) {
  int id = users_manager_->add_red_user("tomy");
  EXPECT_NE(id, -1);

  std::string red_user_name = users_manager_->red_user_name(id);
  EXPECT_EQ(red_user_name, "red_tomy");
}