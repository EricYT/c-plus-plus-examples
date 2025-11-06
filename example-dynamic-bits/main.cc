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
}

int main() { TestDynamicBitSetFunctional(); }
