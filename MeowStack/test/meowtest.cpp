#include <gtest/gtest.h>
#include <meowstack.hpp>
#include <string>
#include <vector>

using namespace MS;

TEST(MeowStack, CreateStack) {
  MeowStack<int> a;
  EXPECT_EQ(a.empty(), true);
}

TEST(MeowStack, CopyStack) {
  MeowStack<std::string> a;
  std::string str = "hello";
  a.push(str);
  a.push("world");
  a.push("!");
  MeowStack<std::string> b(a);
  while (!a.empty()) {
    EXPECT_EQ(a.top(), b.top());
    a.pop();
    b.pop();
  }
  EXPECT_EQ(a.empty(), b.empty());
}

TEST(MeowStack, MoveStack) {
  MeowStack<std::string> a;
  std::string str = "hello";
  a.push(str);
  a.push("world");
  a.push("!");
  MeowStack<std::string> b = MeowStack<std::string>(a);
  while (!a.empty()) {
    EXPECT_EQ(a.top(), b.top());
    a.pop();
    b.pop();
  }
  EXPECT_EQ(a.empty(), b.empty());
}

TEST(MeowStack, PopAndTopFromEmptyStack) {
  MeowStack<int> a;
  EXPECT_EQ(a.empty(), true);
  EXPECT_THROW(a.pop(), std::logic_error);
  EXPECT_THROW(a.top(), std::logic_error);
}

TEST(MeowStack, AnyOtherStackIssue) {
  MeowStack<int> a, c;
  int num = 1;
  a.push(num);
  a.push(2);
  a.push(3);
  a.pop();
  a.top() = 5;
  a.push(4);
  const MeowStack<int> b = a;
  c = MeowStack<int>(b);
  std::vector<int> v = {4, 5, 1};
  int i = 0;
  EXPECT_EQ(b.top(), v[i]);
  while (!a.empty()) {
    EXPECT_EQ(a.top(), v[i]);
    EXPECT_EQ(c.top(), v[i]);
    i++;
    a.pop();
    c.pop();
  }

}