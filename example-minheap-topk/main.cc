#include <cstdint>
#include <functional>
#include <iostream>

#include "min_heap.h"
#include "xassert.h"

template <typename T>
void get_top_k_min_and_display(const MinHeap<T>& h, size_t n) {
  std::vector<T> result;
  top_k_min(h, n, &result);
  std::cout << "top " << n << ":\n";
  for (const auto& v : result) {
    std::cout << v << std::endl;
  }
  std::cout << "top end\n";
}

void TestMinHeapTopK() {
  MinHeap<uint32_t, std::vector<uint32_t>, std::greater<uint32_t>> h;
  h.push(3);
  h.push(1);
  h.push(4);
  h.push(5);
  h.push(2);
  h.push(6);
  h.push(0);
  h.push(10);
  h.push(11);
  h.push(12);
  h.push(12);
  h.push(33);
  h.push(15);

  get_top_k_min_and_display(h, 3);
  get_top_k_min_and_display(h, 8);

  h.pop();
  h.pop();
  h.pop();
  get_top_k_min_and_display(h, 8);
}

struct KeyValue {
  std::string key;
  uint64_t value;

  explicit KeyValue(const std::string& key, uint64_t val)
      : key(key), value(val) {}

  KeyValue(const KeyValue&) = default;
  KeyValue& operator=(const KeyValue&) = default;

  bool operator>(const KeyValue& rhs) const { return value > rhs.value; }
  bool operator<(const KeyValue& rhs) const { return value < rhs.value; }

  friend std::ostream& operator<<(std::ostream& os, const KeyValue& kv);
};

std::ostream& operator<<(std::ostream& os, const KeyValue& kv) {
  os << "{key:" << kv.key << " value:" << kv.value << "}";
  return os;
}

void TestMinHeapTopKWithStruct() {
  MinHeap<KeyValue> mh;
  mh.emplace("a", 1);
  mh.emplace("b", 10);
  mh.emplace("c", 2);
  mh.emplace("d", 3);
  mh.emplace("e", 30);
  mh.emplace("f", 9);
  mh.emplace("g", 19);
  mh.emplace("h", 8);

  get_top_k_min_and_display(mh, 1);
  get_top_k_min_and_display(mh, 3);
  get_top_k_min_and_display(mh, 30);

  mh.pop();
  mh.pop();
  mh.pop();
  mh.pop();
  get_top_k_min_and_display(mh, 3);
}

int main() {
  TestMinHeapTopK();
  TestMinHeapTopKWithStruct();
  return 0;
}
