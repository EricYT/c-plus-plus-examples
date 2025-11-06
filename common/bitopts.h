#pragma once

#include <limits>

inline constexpr unsigned count_leading_zeros(unsigned x) {
  return __builtin_clz(x);
}

inline constexpr unsigned count_leading_zeros(unsigned long x) {
  return __builtin_clzl(x);
}

inline constexpr unsigned count_leading_zeros(unsigned long long x) {
  return __builtin_clzll(x);
}

inline constexpr unsigned count_trailing_zeros(unsigned x) {
  return __builtin_ctz(x);
}

inline constexpr unsigned count_trailing_zeros(unsigned long x) {
  return __builtin_ctzl(x);
}

inline constexpr unsigned count_trailing_zeros(unsigned long long x) {
  return __builtin_ctzll(x);
}

template <typename T>
inline constexpr unsigned log2ceil(T x) {
  static_assert(std::is_integral<T>::value, "T must be a integral type");
  if (x == 1) {
    return 0;
  }
  return std::numeric_limits<T>::digits - count_leading_zeros(x - 1);
}

template <typename T>
inline constexpr unsigned log2floor(T x) {
  return std::numeric_limits<T>::digits - count_leading_zeros(x) - 1;
}
