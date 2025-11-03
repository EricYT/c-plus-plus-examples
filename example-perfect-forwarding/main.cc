#include <cassert>
#include <cerrno>
#include <iostream>
#include <utility>

// test 1
class Bar {
 public:
  Bar(int& i) : iv_(i) { std::cout << "Bar lvalue construction\n"; }
  Bar(const int& i) : iv_(i) { std::cout << "Bar rvalue construction\n"; }
  Bar(int&& i) : iv_(i) { std::cout << "Bar construction\n"; }
  ~Bar() = default;

 private:
  int iv_;
};

void Process(Bar& b) {
  (void)b;
  std::cout << "Process with reference\n";
}

void Process(const Bar& b) {
  (void)b;
  std::cout << "Process with const reference\n";
}

void Process(Bar&& b) {
  (void)b;
  std::cout << "Process with rvalue\n";
}

template <typename T>
void forward_param(T t) {
  std::cout << "forward_param\n";
  Process(t);
}

template <typename T>
void forward_param2(T&& t) {  // universal reference
  std::cout << "forward_param2\n";
  Process(t);
}

template <typename T>
void forward_param3(T&& t) {  // universal reference
  std::cout << "forward_param3\n";
  Process(std::forward<T>(t));
}

// test 2
template <typename T, typename... Args>
T* create(Args&&... args) {  // universal reference
  std::cout << "create\n";
  return new T(std::forward<Args>(args)...);  // perfect forwarding
}

class Foo {
 public:
  Foo(int& i) : iv_(i) { std::cout << "lvalue construction\n"; }
  Foo(int&& i) : iv_(std::move(i)) { std::cout << "rvalue construciton\n"; }

 private:
  int iv_;
};

void TestPerfectForwarding() {
  std::cout << "Start test\n";
  std::cout << "forward_param test\n";
  int a = 10;
  forward_param(a);
  forward_param(10);
  forward_param(std::move(a));

  std::cout << "forward_param2 test\n";
  int b = 20;
  forward_param2(b);
  forward_param2(20);
  forward_param2(std::move(b));

  std::cout << "forward_param3 test\n";
  int c = 30;
  forward_param3(
      c);  // pass lvalue -> T means int& -> forward int& -> call Process(int&)
  forward_param3(30);  // pass rvalue -> T means int -> forward int&& -> call
                       // Process(int&&)
  forward_param3(std::move(c));  // same above

  const int d = 40;
  forward_param3(d);  // pass const lvalue -> T means const int& -> forward
                      // const int& -> call Process(const int&)
}

void TestPerfectForwarding1() {
  std::cout << "Start test1\n";
  int a = 10;
  Foo* f0 = create<Foo>(a);   // forward lvalue
  Foo* f1 = create<Foo>(10);  // forwrad rvalue
  delete f0;
  delete f1;
}

int main() {
  TestPerfectForwarding();
  TestPerfectForwarding1();
}
