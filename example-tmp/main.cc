#include <cassert>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include "defer.h"
#include "mem.h"
#include "tmp.h"
#include "xassert.h"

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

  std::vector<uint32_t> pos;
  for (uint32_t i = 0; i < 100; ++i) {
    pos.emplace_back(i);
  }
  for (auto id : pos) {
    std::cout << id << std::endl;
  }
}

void TestLambdaCapture1() {
  struct Foo {
    uint32_t value;
    explicit Foo(uint32_t v) : value(v) {}
  };

  auto* fp = new Foo(124U);
  DEFER({ delete fp; });
  uint32_t u = 123;
  auto md = [=]() {
    std::cout << "foo value:" << (fp->value++) << std::endl;
    std::cout << "u:" << u << std::endl;
  };
  std::cout << "before cb\n";
  md();
  std::cout << "after cb\n";
  std::cout << "foo value:" << fp->value++ << std::endl;
  std::cout << "u:" << u++ << std::endl;
}

void TestIterateTemplate() {
  std::vector<uint32_t> foo{0, 1, 2, 3};
  std::cout << "iterate begin:\n";
  for_each(foo, [](uint32_t v) { std::cout << v << std::endl; });
  std::cout << "iterate end.\n";

  std::list<uint32_t> bar;
  bar.assign(foo.begin(), foo.end());
  std::cout << "iterate begin:\n";
  for_each_v1(bar.begin(), bar.end(),
              [](uint32_t v) { std::cout << v << std::endl; });
  std::cout << "iterate end.\n";
}

void TestFillup() {
  constexpr uint32_t BUFF_SIZE = 4096;
  uint32_t size = 1048575;
  uint32_t offset = 10245;

  uint32_t write_off = offset;
  uint32_t remain_size = size;
  while (remain_size > 0) {
    uint32_t buff_index = write_off / BUFF_SIZE;
    uint32_t buff_off = write_off % BUFF_SIZE;
    uint32_t buff_len = std::min(BUFF_SIZE - buff_off, remain_size);
    std::cout << "write_off:" << write_off << " buff_index:" << buff_index
              << " len:" << buff_len << " remain_size:" << remain_size
              << std::endl;
    write_off += buff_len;
    remain_size -= buff_len;
  }
  ASSERT(remain_size == 0);
}

extern "C" {

void convert_member_pointer_to_struct() {
  struct Foo {
    uint8_t key;
    uint32_t value;
    uint64_t desc;
  };
  printf("key of Foo: %lu\n", offsetof(Foo, key));
  printf("value of Foo: %lu\n", offsetof(Foo, value));
  printf("desc of Foo: %lu\n", offsetof(Foo, desc));

  Foo f;
  f.key = 1;
  f.value = 2;
  f.desc = 3;
  void* dummy = &f.value;
  Foo* fp = container_of(dummy, struct Foo, value);
  if (fp != &f) {
    printf("Convert member ptr failed\n");
    abort();
  }
}
};

void TestOffsetOf() {
  convert_member_pointer_to_struct();

  struct Bar {
    uint8_t key;
    uint32_t value;
    uint8_t desc;

    explicit Bar(uint8_t k, uint32_t v, uint8_t d)
        : key(k), value(v), desc(d) {}
  };

  Bar b(1, 2, 3);
  std::cout << "key of Bar:" << offsetof(Bar, key) << std::endl;
  std::cout << "value of Bar:" << offsetof(Bar, value) << std::endl;
  std::cout << "desc of Bar:" << offsetof(Bar, desc) << std::endl;

  Bar* bp = get_struct_ptr(&b.desc, &Bar::desc);
  ASSERT(bp == &b);

  Bar* bp1 = reinterpret_cast<Bar*>(reinterpret_cast<char*>(&b.desc) -
                                    offsetof(Bar, desc));
  ASSERT(bp1 == &b);
}

void TestIndexWithArray() {
  struct Foo {
    uint8_t key_;
    uint32_t value_;
  };

  constexpr uint32_t FOO_SIZE = 10;
  auto* fp = new Foo[FOO_SIZE];
  DEFER({ delete[] fp; });

  ASSERT((&fp[5] - fp) == 5);
  ASSERT((&fp[0] - fp) == 0);
}

void TestCast() {
  struct Foo {
    std::string key;
    std::string value;
  };

  struct Bar : public Foo {
    uint32_t index;
  };

  Bar b;
  b.key = "a";
  b.value = "b";
  b.index = 9527;

  Foo* foo = &b;

  auto bp = reinterpret_cast<Bar*>(foo);
  ASSERT(bp->index == 9527);
  ASSERT(bp->key == "a");
  ASSERT(bp->value == "b");
}

void TestStructEnum() {
  struct Foo {
    enum class Bar : uint8_t { BAR_0 = 0, BAR_1, BAR_2 };

    std::string key;
    Bar bar;

    bool IsBar1() const { return bar == Bar::BAR_1; }

    std::string BarToString(Bar b) {
      switch (b) {
        case Bar::BAR_0:
          return "BAR0";
        case Bar::BAR_1:
          return "BAR_1";
        case Bar::BAR_2:
          return "BAR_2";
        default:
          return "UNKNOWN";
      }
    }
  };
}

void TestSizeofWithVector() {
  struct Bar {
    uint32_t a;
    uint8_t b;
  };

  struct Foo {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    std::vector<Bar> bars;
  };

  struct Empty {
    uint32_t a;
    // empty std::vecotr use 32 bytes
    std::vector<Foo> foos;
  };
  std::vector<uint32_t> empty_vector;
  std::cout << "Foo size:" << sizeof(Foo) << " Bar size:" << sizeof(Bar)
            << " Empty size:" << sizeof(Empty)
            << " Empty std::vector size:" << sizeof(empty_vector) << std::endl;
}

class BindClass {
 public:
  std::function<void()> GetPrivateBindFun() {
    return std::bind(&BindClass::PrivateFunc, this);
  }

 private:
  void PrivateFunc() { std::cout << "BinadClass PrivateFunc called\n"; }
};

void TestBindPrivateFunc() {
  BindClass bc;
  auto func = bc.GetPrivateBindFun();
  func();
}

struct FooWithDefault {
  uint32_t a = 0;
  uint32_t b = 323;
  uint32_t c = 0;

  FooWithDefault() = default;

  FooWithDefault(uint32_t c) : c(c) {}

  std::string ToString() const {
    std::ostringstream oss;
    oss << " a:" << a;
    oss << " b:" << b;
    oss << " c:" << c;
    return oss.str();
  }
};

void TestStructDefault() {
  FooWithDefault foo(123);
  std::cout << "Foo " << foo.ToString() << std::endl;
}

void TestTmp() {
  TestStdSet();
  TestWrapperCallback();
  TestCompare();
  TestXX();
  TestIterateUnorderedMapByBucket();
  TestMapIterator();
  TestLambdaCapture();
  TestLambdaCapture1();
  TestIterateTemplate();
  TestFillup();
  TestOffsetOf();
  TestIndexWithArray();
  TestCast();
  TestStructEnum();
  TestSizeofWithVector();
  TestBindPrivateFunc();
  TestStructDefault();
}

int main() {
  TestTmp();
  return 0;
}
