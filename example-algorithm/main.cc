#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <set>
#include <string>

#include "debug.h"
#include "slice.h"

struct BySmallestKey {
  // NOTE: operator() must be a const function
  bool operator()(Slice* s1, Slice* s2) const {
    DEBUG("s1: %s s2: %s\n", s1->to_string().c_str(), s2->to_string().c_str());
    return s1->compare(*s2) < 0;
  }
};

typedef std::set<Slice*, BySmallestKey> SliceSet;

void TestAlgorithm() {
  std::cout << "Algorithm test start\n";
  BySmallestKey cmp;
  SliceSet ss(cmp);

  Slice s1("abc1");
  Slice s2("a");
  Slice s3("bc");

  ss.insert(&s1);
  ss.insert(&s2);
  ss.insert(&s3);

  for (const auto* s : ss) {
    std::cout << s->to_string() << std::endl;
  }

  Slice tmp("def");
  auto pos = std::upper_bound(ss.begin(), ss.end(), &tmp, cmp);
  // auto pos = ss.upper_bound(&tmp);  // this way we can ignore the cmp
  // operator
  std::cout << tmp.to_string()
            << " upper bound pos: " << std::distance(ss.begin(), pos)
            << std::endl;
}

int main() {
  TestAlgorithm();
  return 0;
}
