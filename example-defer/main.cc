#include <iostream>
#include <string>

#include "defer.h"

void TestDefer() {
  std::cout << "Defer test start\n";
  {
    std::cout << "Before Defer\n";
    DEFER(std::cout << "Defer 1\n");
    DEFER(std::cout << "Defer 2\n");
    std::cout << "After Defer\n";
  }
  std::cout << "Defer test end\n";
}

class Callable {
 public:
  Callable() = default;
  ~Callable() = default;

  // implement operator (), so defer can call it by operator ()
  void operator()() { std::cout << "Callable called\n"; }

 private:
};

void TestDeferV1() {
  std::cout << "Defer test start v1\n";
  {
    std::cout << "Defer test before\n";
    // must assign defer result to a lvalue, otherwise defer called
    // deconstruction after line 22 IF we don't get the result, defer returns
    // treated temporary object, after defer called, the value deconstruction
    auto _a = defer([]() { std::cout << "Defer 1\n"; });
    auto _b = defer([]() { std::cout << "Defer 2\n"; });
    std::cout << "Defer test after\n";
  }
  {
    std::cout << "Defer test before callable\n";
    auto _ = defer(Callable{});
    std::cout << "Defer test after callable\n";
  }
  std::cout << "Defer test end v1\n";
}

int main() {
  TestDefer();
  TestDeferV1();
  return 0;
}
