#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

struct ILog {
  virtual bool log(const std::string& text) = 0;
};

// real object
class FileLogger : public ILog {
  bool log(const std::string& text) override {
    std::string file_path = "log.txt";

    std::ofstream file(
      file_path.data(), std::fstream::out | std::fstream::app);
    if (file.is_open() == false){
      return false;
    }
    file << text;
    return true;
  }
};

/*
 * dumy(test double)
 *
 * do not anything but
 * just do parameter role in test process
 */
class DumyLogger : public ILog { 
public:
  bool log(const std::string& text) override {
    //have not logic
    std::cout << text << std::endl;
    return true;
  }
};

// test target
class Game { 
  ILog* logger_;
public:
  Game(ILog* logger) 
    : logger_(logger) {}
  bool init() { 
    if (logger_->log("initialize game setting") == false) {
      return false;
    }

    return true;
  }
  bool play() { 
    if (logger_->log("play game") == false) {
      return false;
    }

    return true;
  }
};

class DumyTest : public ::testing::Test {
protected:
  Game* game_;
  virtual void SetUp() override { 
    game_ = new Game(new DumyLogger()); 
  }

  virtual void TearDown() override { 
    delete game_; 
  }
};

TEST_F(DumyTest, init) {
  ASSERT_TRUE(game_->init());
}

TEST_F(DumyTest, play) {
  EXPECT_TRUE(game_->play());
}