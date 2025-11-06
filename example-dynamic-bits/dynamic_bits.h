#ifndef __EXAMPLE_DYNAMIC_BITS__
#define __EXAMPLE_DYNAMIC_BITS__

#include <cstddef>
#include <limits>
#include <vector>

#include "bitopts.h"

class DynamicBits {
 public:
  explicit DynamicBits(size_t nbits);

 private:
  using int_type = uint64_t;
  static constexpr size_t bits_per_int = std::numeric_limits<int_type>::digits;
  static constexpr int_type all_set = std::numeric_limits<int_type>::max();
  static constexpr unsigned level_shift = log2ceil(bits_per_int);

  // bits all in bits_[0]
  // bits_[N...] for fast indexing
  std::vector<std::vector<int_type>> bits_;
  size_t bits_cnt_;
};

#endif  // __EXAMPLE_DYNAMIC_BITS__
