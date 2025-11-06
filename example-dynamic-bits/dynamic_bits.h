#ifndef __EXAMPLE_DYNAMIC_BITS__
#define __EXAMPLE_DYNAMIC_BITS__

#include <cstddef>
#include <limits>
#include <vector>

#include "bitopts.h"

class DynamicBits {
 public:
  explicit DynamicBits(size_t nbits);

  bool test(size_t n) const noexcept {
    auto idx = n / bits_per_int;
    return bits_[0][idx] & (int_type(1U) << (n % bits_per_int));
  }

  void set(size_t n) noexcept;

  void clear(size_t n) noexcept;

  size_t size() const noexcept { return nr_bits_; }

  size_t find_first_set() const noexcept;

  size_t find_next_set(size_t n) const noexcept;

  size_t find_last_set() const noexcept;

  enum : size_t { nopos = std::numeric_limits<size_t>::max() };

 private:
  using int_type = uint64_t;
  static constexpr size_t bits_per_int = std::numeric_limits<int_type>::digits;
  static constexpr int_type all_set = std::numeric_limits<int_type>::max();
  static constexpr unsigned level_shift = log2ceil(bits_per_int);

  // bits all in bits_[0]
  // bits_[N...] for fast indexing
  std::vector<std::vector<int_type>> bits_;
  size_t nr_bits_;
};

#endif  // __EXAMPLE_DYNAMIC_BITS__
