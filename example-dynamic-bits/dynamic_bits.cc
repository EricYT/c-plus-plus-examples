#include "dynamic_bits.h"

#include <ranges>

#include "align.h"

DynamicBits::DynamicBits(size_t nr_bits) : nr_bits_(nr_bits) {
  auto div_ceil = [](unsigned num, unsigned den) {
    return (num + den - 1) / den;
  };
  // [1-64]: level 1
  // [65-4096]: level 2
  // [4097-262144]: level 3
  // etc
  unsigned nr_levels =
      div_ceil(log2ceil(align_up(nr_bits, bits_per_int)), level_shift);
  bits_.resize(nr_levels);
  size_t level_bits = nr_bits;
  for (unsigned i = 0; i != nr_levels; ++i) {
    unsigned level_words = align_up(level_bits, bits_per_int) / bits_per_int;
    bits_[i].resize(level_words);
    level_bits = level_words;  // for next level iteration
  }
}

void DynamicBits::set(size_t n) noexcept {
  for (auto& level : bits_) {
    unsigned idx = n / bits_per_int;
    auto old = level[idx];
    level[idx] |= (int_type(1U) << (n % bits_per_int));
    if (old) {
      break;
    }
    n = idx;  // prepare for next level
  }
}

void DynamicBits::clear(size_t n) noexcept {
  for (auto& level : bits_) {
    unsigned idx = n / bits_per_int;
    auto old = level[idx];
    level[idx] &= ~(int_type(1U) << (n % bits_per_int));
    if (!old || level[idx]) {
      break;
    }
    n = idx;  // prepare for next level
  }
}

size_t DynamicBits::find_first_set() const noexcept {
  size_t pos = 0;
  for (auto it = bits_.rbegin(); it != bits_.rend(); ++it) {
    const auto& vv = *it;
    auto v = vv[pos];
    pos *= bits_per_int;
    if (v) {
      pos += count_trailing_zeros(v);
    } else {
      return nopos;
    }
  }
  return pos;
}

size_t DynamicBits::find_next_set(size_t n) const noexcept {
  (void)n;
  return 0;
}

size_t DynamicBits::find_last_set() const noexcept {
  size_t pos = 0;
  for (auto it = bits_.rbegin(); it != bits_.rend(); ++it) {
    const auto& vv = *it;
    auto v = vv[0];
    pos *= bits_per_int;
    if (v) {
      pos += bits_per_int - 1 - count_leading_zeros(v);
    } else {
      return nopos;
    }
  }
  return pos;
}
