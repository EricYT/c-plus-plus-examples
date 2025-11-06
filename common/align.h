#pragma once

#include <cstdint>
#include <cstdlib>

#include "assert.h"

template <typename T>
inline constexpr T align_up(T v, T align) {
  ASSERT(((align & (align - 1)) == 0) && "align must be power of two");
  return (v + align - 1) & ~(align - 1);
}

template <typename T>
inline constexpr T* align_up(T* v, T align) {
  ASSERT(((align & (align - 1)) == 0) && "align must be power of two");
  static_assert(sizeof(T) == 1, "align byte pointers only");
  return reinterpret_cast<T*>(align_up(static_cast<uintptr_t>(v), align));
}

template <typename T>
inline constexpr T align_down(T v, T align) {
  ASSERT(((align & (align - 1)) == 0) && "align must be power of two");
  return v & ~(align - 1);
}

template <typename T>
inline constexpr T* align_down(T* v, T align) {
  ASSERT(((align & (align - 1)) == 0) && "align must be power of two");
  static_assert(sizeof(T) == 1, "align byte pointers only");
  return reinterpret_cast<T*>(static_cast<uintptr_t>(v), align);
}
