#ifndef __EXAMPLE_TMP_TMP_H
#define __EXAMPLE_TMP_TMP_H

#include <iostream>
#include <unordered_map>

class Hi {
 public:
  Hi() {}
  virtual ~Hi() { std::cout << "Hi deconstruction" << std::endl; }

  virtual void SayHi();
};

class Hello : public Hi {
 public:
  Hello() = default;
  ~Hello();

  void SayHi() override;
};

class MapList {
 public:
  MapList() = default;
  ~MapList();

  void Push(uint32_t key, uint32_t val);

  void Run();

 private:
  std::unordered_map<uint32_t, uint32_t> kvs_;
};

void SetPerRoundMaxItemCnt(uint32_t cnt);

template <typename T, typename UnaryFunc>
void for_each(const T& t, UnaryFunc f) {
  for (auto& item : t) {
    f(item);
  }
}

template <typename Input, typename UnaryFunc>
constexpr UnaryFunc for_each_v1(Input first, Input last, UnaryFunc f) {
  for (; first != last; ++first) {
    f(*first);
  }
  return f;
}

#endif  // __EXAMPLE_TMP_TMP_H
