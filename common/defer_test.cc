#include "defer.h"

#include <vector>

#include "gtest/gtest.h"

namespace example {

TEST(DEFER, Defer) {
  int a = 10;
  {
    DEFER({ a = 20; });
    DEFER({ a = 30; });
    ASSERT_EQ(a, 10);
  }
  ASSERT_EQ(a, 20);
}

}  // namespace example
