#include <iostream>

#include "dynamic_bits.h"
#include "xassert.h"

void TestDynamicBitSetFunctional() {
  DynamicBits bits(10000);
  ASSERT(!bits.test(0));
  bits.set(0);
  ASSERT(bits.test(0));
  bits.clear(0);
  ASSERT(!bits.test(0));

  ASSERT(!bits.test(9527));
  bits.set(9527);
  ASSERT(bits.test(9527));

  size_t pos = bits.find_first_set();
  ASSERT(pos == 9527);

  bits.set(231);
  pos = bits.find_first_set();
  ASSERT(pos == 231);

  pos = bits.find_last_set();
  ASSERT(pos == 9527);
  bits.clear(9527);
  ASSERT(!bits.test(9527));
  pos = bits.find_last_set();
  ASSERT(pos == 231);
}

void TestDynamicBitSetFindNext() {
  DynamicBits bits(10000);
  bits.set(0);
  bits.set(64);
  bits.set(4095);
  bits.set(4096);
  bits.set(9527);
  bits.set(8000);

  ASSERT(bits.find_next_set(-1) == 0);
  ASSERT(bits.find_next_set(0) == 64);
  ASSERT(bits.find_next_set(64) == 4095);
  ASSERT(bits.find_next_set(4095) == 4096);
  ASSERT(bits.find_next_set(4096) == 8000);
  ASSERT(bits.find_next_set(8000) == 9527);
  ASSERT(bits.find_next_set(9527) == DynamicBits::nopos);
}

int main() {
  TestDynamicBitSetFunctional();
  TestDynamicBitSetFindNext();
  return 0;
}
