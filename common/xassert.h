#pragma once

// independent of NDEBUG. Active in all mode

#ifdef __APPLE__
#include <assert.h>
#define ASSERT(x)                                                \
  do {                                                           \
    if (!(x)) {                                                  \
      __assert_rtn(#x, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
    }                                                            \
  } while (0)
#elif
#include <cassert>
define ASSERT(x) do {
  if (!(x)) {
    __assert_fail(#x, __FILE__, __LINE__, __PRETTY_FUNCTION__);
  }
}
while (0)
#endif
