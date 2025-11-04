#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <string>

void TestStdSet() {
  struct Foo {
    uint64_t key;
    std::string value;

    explicit Foo(uint64_t key, const std::string& value)
        : key(key), value(value) {}
  };

  struct FooCompare {
    bool operator()(const Foo& a, const Foo& b) const { return a.key < b.key; }
  };

  using FooSet = std::set<Foo, FooCompare>;

  FooSet fs;
  assert(fs.empty());

  fs.emplace(0, "hello");
  fs.emplace(1, "wolrd");
  assert(fs.size() == 2U);

  fs.emplace(1, "foo");
  assert(fs.size() == 2U);
}

void TestTmp() { TestStdSet(); }

int main() {
  TestTmp();
  return 0;
}
