#include <functional>
#include <iostream>
#include <string>

class Defer {
  public:
    explicit Defer(std::function<void()> func) : func_(std::move(func)) {}
    ~Defer() { if (func_) func_(); }

    Defer(const Defer&) = delete;
    Defer& operator=(const Defer&) = delete;

  private:
    std::function<void()> func_;
};

#define CONCAT(x, y) CONCAT2(x, y)
#define CONCAT2(x, y) x##y
#define DEFER(...) Defer CONCAT(__defer_, __LINE__)([]() { __VA_ARGS__; })

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

int main() {
  TestDefer();
  return 0;
}
