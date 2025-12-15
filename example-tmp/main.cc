#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "tmp.h"

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

using RealCallback = std::function<void(int)>;

void Callback(int r) { std::cout << "Callback r:" << r << std::endl; }

void CallbackWrapper(RealCallback cb, int r) {
  std::cout << "CallbackWrapper r:" << r << std::endl;
  cb(r);
}

void CallWithCallback(RealCallback callback) {
  auto wrapper_cb = [=](int r) {
    std::cout << "wrapper callback r:" << r << std::endl;
    callback(r);
  };
  wrapper_cb(1);
}

void CallWithCallbackBind(RealCallback callback) {
  auto wrapper_cb = std::bind(CallbackWrapper, callback, std::placeholders::_1);
  wrapper_cb(1);
}

void TestWrapperCallback() {
  CallWithCallback(Callback);
  CallWithCallbackBind(Callback);
}

struct Item {
  uint32_t val;

  explicit Item(uint32_t v) : val(v) {}

  constexpr bool operator()(const Item& rhs) { return val < rhs.val; }

  friend std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << "Item val:" << item.val;
    return os;
  }
};

void TestCompare() {
  std::map<uint32_t, Item> items;
  items.emplace(0, 1);
  items.emplace(1, 20);
  items.emplace(2, 12);
  items.emplace(3, 0);
  for (auto& item : items) {
    std::cout << "Key:" << item.first << " item:" << item.second << std::endl;
  }
}

void TestXX() {
  Hello h;
  h.SayHi();

  std::vector<uint32_t> vsc{0, 1, 3, 2, 9, 10, 20, 4, 7};
  vsc.erase(
      std::remove_if(vsc.begin(), vsc.end(), [](uint32_t v) { return v < 7; }),
      vsc.end());
  for (auto& v : vsc) {
    std::cout << "after remove: " << v << std::endl;
  }

  std::unordered_map<uint32_t, uint32_t> map;
  map[0] = 1;
  const auto& iter = map.find(0);
  if (iter == map.end()) {
    std::cout << "xxx" << std::endl;
  }
  std::cout << "key:" << iter->first << " value:" << iter->second << std::endl;
}

void TestIterateUnorderedMapByBucket() {
  MapList ml;
  ml.Run();

  ml.Push(0, 1);
  ml.Run();

  for (uint32_t i = 0; i < 15; ++i) {
    ml.Push(i, i + 1);
  }
  ml.Run();
  ml.Run();
  SetPerRoundMaxItemCnt(20);
  ml.Run();
  ml.Run();
}

void TestMapIterator() {
  std::unordered_map<uint32_t, uint32_t> map;
  map[0] = 1;
  map[1] = 2;
  // map element is std::pair<key, value>
  // iterate with : dereference to std::pair<key, vlaue>
  // so we just need to use item.first
  for (const auto& item : map) {
    std::cout << "key:" << item.first << " value:" << item.second << std::endl;
  }
  // iterate with iterator, iterator is something like pointer to
  // std::pair<key, value> so we have to reference by iter->first
  // or (*iter).first
  for (auto iter = map.cbegin(); iter != map.cend(); ++iter) {
    std::cout << "key:" << iter->first << " value:" << iter->second
              << std::endl;
  }
}

void TestLambdaCapture() {
  uint32_t last_pos = 0;
  uint32_t curr_pos = 0;
  for (uint32_t i = 0; i < 5; ++i) {
    std::cout << "before curr_pos:" << curr_pos << " last_pos:" << last_pos
              << std::endl;
    auto func = [&]() {
      last_pos = curr_pos;
      for (uint32_t j = 0; j < 4; j++) {
        ++curr_pos;
      }
    };
    func();
  }
}

void TestTmp() {
  TestStdSet();
  TestWrapperCallback();
  TestCompare();
  TestXX();
  TestIterateUnorderedMapByBucket();
  TestMapIterator();
  TestLambdaCapture();
}

int main() {
  TestTmp();
  return 0;
}
