#include <thread>
#include <gtest/gtest.h>

template <int N>
class TimeCriticalTest : public ::testing::Test {
protected:
  std::chrono::system_clock::time_point start_time;
  static const int PIVOT = N;
  virtual void SetUp() override { 
    start_time = std::chrono::system_clock::now();
  }
  virtual void TearDown() override {
    auto end_time = 
      std::chrono::system_clock::now();
    auto duration_time = 
      std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    EXPECT_TRUE(duration_time < PIVOT) << "timeout :" << duration_time;
  }
};

//timeout test class
class SECOND_TEST_2 : public TimeCriticalTest<2> {};
class SECOND_TEST_3 : public TimeCriticalTest<3> {};

// //timeout test suite
// TEST_F(SECOND_TEST_2, Sample) { 
//   std::this_thread::sleep_for(std::chrono::seconds(1));
// }

// TEST_F(SECOND_TEST_3, Sample) { 
//   std::this_thread::sleep_for(std::chrono::seconds(2));
// }