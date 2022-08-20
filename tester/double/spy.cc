#include <gtest/gtest.h>
#include <not-implemented-exception.hh>
#include <iostream>

struct IReporter {
  virtual void report(const std::string& content) = 0;
};

// real object
class ErrorReporter : public IReporter {
  void report(const std::string& content) override {
    throw NotImplementedException();
  }
};

/*
 * spy(test double)
 *
 * check whether a function is called or
 * check whether a function is called a certain number of times
 */
class SpyReporter : public IReporter { 
public:
  int count_ = 0;
  void report(const std::string& content) override {
    ++count_;
  }
};

// test target
class Robot {
private:
  IReporter* reporter_;

public:
  Robot(IReporter* reporter) 
    : reporter_(reporter) {}
  bool command(const std::string& cmd) { 
    if (cmd == "cmd1" || 
        cmd == "cmd2" || 
        cmd == "cmd2") {
      std::cout << "do " << cmd << std::endl;
      return true;
    }

    reporter_->report("wrong command : " + cmd);
    return false;
  }
  IReporter* reporter() {
    return reporter_;
  }
};

class SpyTest : public ::testing::Test {
protected:
  Robot* robot_;
  virtual void SetUp() override { 
    robot_ = new Robot(new SpyReporter()); 
  }

  virtual void TearDown() override { 
    delete robot_; 
  }
};

TEST_F(SpyTest, error_report) {
  robot_->command("wrong cmd");
  auto reporter = robot_->reporter();
  auto error_reporter = static_cast<SpyReporter*>(reporter);
  ASSERT_EQ(error_reporter->count_, 1);
}