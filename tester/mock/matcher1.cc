#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class BaseDoDumy {
public:
  int data;
};

class DerrivedDoDumy : public BaseDoDumy {
};

class IDoSomething {
public:
  virtual char do_this(int n) = 0;
  virtual void do_that(const char* s, int* p) = 0;
  virtual void do_some(int* n) = 0;
};

class MockDoSomething : public IDoSomething {
public:
  MOCK_METHOD(char, do_this, (int n), (override));
  MOCK_METHOD(void, do_that, (const char* s, int* p), (override));
  MOCK_METHOD(void, do_some, (int* n), (override));

  MOCK_METHOD(void, print, (int n), ());
  MOCK_METHOD(void, print, (char c), ());

  MOCK_METHOD(void, do_two, (int a, int b), ());
  MOCK_METHOD(void, do_three, (int a, int b, int c), ());
  MOCK_METHOD(void, do_string, (std::string a), ());
  MOCK_METHOD(void, do_casting, (DerrivedDoDumy* a), ());
  MOCK_METHOD(void, do_container, (const std::vector<int>& a), ());
  MOCK_METHOD(void, do_container2, ((const std::map<std::string, int>& a)), ());
};

/*
 * matcher : Filtering on mock method arguments
 *
 * The mock method that puts parameters has matcher applied by default.
 */
TEST(GMOCK_MATCHER_TEST, do_this_default) {
  MockDoSomething mock;
  int bar = 10;
  EXPECT_CALL(mock, do_this(5))//here is matcher. check parameter value is 5
    .WillOnce(::testing::Return('a'));//if 5 is input then return 'a'
  EXPECT_CALL(mock, do_that("Hello", &bar));//matcher를 이용하여 인자값이 맞는지 검사

  //mock.do_this(12);//fail
  EXPECT_EQ(mock.do_this(5), 'a');//ok

  int bar_copy = 10;
  // mock.do_that("Hello", &bar_copy);//fail
  mock.do_that("Hello", &bar);//ok
}

/*
 * Ge, NotNull matcher
 *
 * and there are more matcher
 * AllOf(), AllOfArray(), AnyOf(), AnyOfArray(), Not()
 * you can combine matcher by using AllOf()
 */
TEST(GMOCK_MATCHER_TEST, do_this_some) {
  MockDoSomething mock;

  EXPECT_CALL(mock, do_this(::testing::Ge(5))) //1번째 인자는 >=5
    .WillOnce(::testing::Return('a'));
  EXPECT_CALL(mock, do_that(::testing::_, ::testing::NotNull()));
  EXPECT_CALL(mock, do_that("Hello", ::testing::NotNull()));//2번째 인자는 NULL이면 안됨

  EXPECT_EQ(mock.do_this(5), 'a');

  int bar_copy = 10;
  mock.do_that("Hello", &bar_copy);
  mock.do_that("any", &bar_copy);
}

/*
 * combine and variable
 */
 TEST(GMOCK_MATCHER_TEST, combine_and_variable) {
  using ::testing::AllOf;
  using ::testing::Gt;
  using ::testing::Le;
  using ::testing::Matcher;
  using ::testing::Not;
  using ::testing::HasSubstr;
  using ::testing::Contains;
  using ::testing::Property;

  MockDoSomething mock;
  Matcher<int> in_range = AllOf(Gt(5), Le(10));// 5 < x < 10
  EXPECT_CALL(mock, do_this(in_range))
    .WillOnce(::testing::Return('a'));  
  
  EXPECT_CALL(mock, do_that(Not(HasSubstr("blah")), NULL));
                            
  EXPECT_EQ(mock.do_this(7), 'a');
  mock.do_that("abcd", NULL);
  // mock.do_that("**blah**", NULL); // fail\]
}

// pointee
TEST(GMOCK_MATCHER_TEST, pointee) {
  MockDoSomething mock;
  EXPECT_CALL(mock, 
    do_some(
      ::testing::Pointee(::testing::Ge(5))
    )
  );

  //also enable : Pointee(Pointee(Lt(3)))

  EXPECT_CALL(mock, 
    do_that(::testing::IsNull(), ::testing::NotNull()));

  int data = 10;
  mock.do_some(&data);
  mock.do_that(nullptr, &data);
}

/*
 * cating matcher
 *
 * It prevents implicit casting and raises a compilation error if the type does not match exactly.
 */
TEST(GMOCK_MATCHER_TEST, casting) {
  using ::testing::SafeMatcherCast;
  using ::testing::Pointee;
  using ::testing::Eq;

  MockDoSomething mock;
  // m is a Matcher<Base*> we got from somewhere.
  
   DerrivedDoDumy* derrived = new DerrivedDoDumy();// test fail
  // BaseDoDumy* derrived = new DerrivedDoDumy();// test fail
  // 142 |   mock.do_casting(derrived);
  //     |                   ^~~~~~~~
  //     |                   |
  //     |                   BaseDoDumy*  
  EXPECT_CALL(mock, 
    do_casting(
      SafeMatcherCast<DerrivedDoDumy*>(
        Eq(derrived)
      )
    )
  );

  mock.do_casting(derrived);
}

// you can use overload mock method by using matcher
TEST(GMOCK_MATCHER_TEST, overload) {
  MockDoSomething mock;
  EXPECT_CALL(mock, print(::testing::An<int>()));
  EXPECT_CALL(mock, print(::testing::Matcher<int>(::testing::Lt(5))));
  EXPECT_CALL(mock, print(::testing::TypedEq<char>('a')));

  mock.print(3);
  mock.print(6);
  mock.print('a');
}

// select action by input argument
TEST(GMOCK_MATCHER_TEST, select_action_by_arg) {
  MockDoSomething mock;

  // default return 'b'
  EXPECT_CALL(mock, do_this(::testing::_))
    .WillRepeatedly(::testing::Return('b'));

  // if less 5 then return 'a'
  EXPECT_CALL(mock, do_this(::testing::Lt(5)))
    .WillRepeatedly(::testing::Return('a'));
  
  //less 5 so return 'a'
  EXPECT_EQ(mock.do_this(3), 'a');
}

// compare args
TEST(GMOCK_MATCHER_TEST, compare_args) {
  using ::testing::AllOf;
  using ::testing::Args;
  using ::testing::AllArgs;
  using ::testing::Lt;
  using ::testing::Gt;

  MockDoSomething mock;
  EXPECT_CALL(mock, do_three) // #1
    .With(
      AllOf(
        Args<0, 1>(Lt()), //args[0] < args[1]
        Args<1, 2>(Gt())  //args[1] > args[2]
      )
    );

  EXPECT_CALL(mock, do_two) // #2
    .With(
      //type 1
      // AllOf(
      //   Args<0, 1>(Lt())
      // )

      // type 2, args length should be 2    
      // AllArgs(Lt())
      
      // type 3, args length should be 2
      Lt()
    );    

  mock.do_three(0, 3, 2);// #1 
  mock.do_two(0, 1);// #2
}

// predicate
int is_even(int n) { 
  return (n % 2) == 0 ? 1 : 0; 
}
TEST(GMOCK_MATCHER_TEST, predicate) {
  std::vector<int> v{1,2,3,4,5};
  const int count = 
    std::count_if(v.begin(), v.end(), 
      ::testing::Matches(::testing::Ge(3)));// you use matcher in stl, it is predicate
  
  // should wrap 'Truly'
  // A predicate does not necessarily return a bool. 
  // If the return value can be used in if (condition) it can be used
  MockDoSomething mock;
  EXPECT_CALL(mock, do_this(::testing::Truly(is_even)));

  EXPECT_EQ(count, 3);
  mock.do_this(2);
}

// refrence
TEST(GMOCK_MATCHER_TEST, refrence) {
  MockDoSomething mock;
  std::string str1 = "abc";
 
  // expects that do_string()'s argument == str1.
  // if arg can't copy, should use ref
  EXPECT_CALL(mock, do_string(::testing::Eq(std::ref(str1))));//also can use Lt()
  // EXPECT_CALL(mock, do_string(::testing::Eq(str1)));//copy str1, test will fail

  str1 = "bcd";

  std::string str2 = "bcd";
  mock.do_string(str2);
}

/*
 * if STL iterator is implemented, it can be used without STL container
 * ElementsAre*() can be nested
 * if you are using a pointer instead of a reference, use Pointee(ElementsAre*(...))
 * the order of elements in ElementsAre*() is important
 * if the element is unsorted (hash_map), you must wrap the ElementsAre with WhenSorted
 */
TEST(EXPECT_TEST, elements_are) {

  std::vector<int> x{2,1,3};
  EXPECT_THAT(x, ::testing::ElementsAre(2, 1, 3));
  // EXPECT_EQ(x[0], 2) << "Vectors x and y differ at index " << 0;
  // EXPECT_EQ(x[1], 1) << "Vectors x and y differ at index " << 1;
  // EXPECT_EQ(x[2], 3) << "Vectors x and y differ at index " << 2;

  std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
  EXPECT_THAT(m, 
    ::testing::ElementsAre(
      ::testing::Pair("a", 1), 
      ::testing::Pair("b", 2), 
      ::testing::Pair("c", 3)));

  // //checks that in vector v all the elements are greater than 10 and less than 20
  // ASSERT_THAT(v, Each(AllOf(Gt(10), Lt(20))));

  // //checks that vector v consist of 
  // //   5, number greater than 10, anything.
  // ASSERT_THAT(v, ElementsAre(5, Gt(10), _));  
}

// container
TEST(GMOCK_MATCHER_TEST, container1) {
  MockDoSomething mock;
  EXPECT_CALL(
    mock, 
    do_container(
      ::testing::ElementsAre(1, ::testing::Gt(0), ::testing::_, 5)));  
      // ::testing::UnorderedElementsAre()

  std::vector<int> v{
    1, // 1
    2, // ::testing::Gt(0)
    -1,// ::testing::_
    5  // 5
  };
  mock.do_container(v);
}

TEST(GMOCK_MATCHER_TEST, container2) {
  MockDoSomething mock;
  // ElementsAreArray accepts an array of element values.
  const int expected_vector1[] = {1, 5, 2, 4};
  EXPECT_CALL(mock, do_container(::testing::ElementsAreArray(expected_vector1)));//vector but can use array
  // Or, an array of element matchers.
  ::testing::Matcher<int> expected_vector2[] = {1, ::testing::Gt(2), ::testing::_, 3};
  EXPECT_CALL(mock, do_container(::testing::ElementsAreArray(expected_vector2)));

  std::vector<int> v1{ 1, 5, 2, 4 };
  mock.do_container(v1);
  std::vector<int> v2{ 1, 3, 0, 3 };
  mock.do_container(v2);
}