#ifndef __COMMON_MEM_H__
#define __COMMON_MEM_H__

#ifdef __cplusplus
#include <type_traits>
#endif
#include <stddef.h>

#include <cstdint>

#ifdef __cplusplus

#define container_of(ptr, type, member)                                  \
  reinterpret_cast<type*>(                                               \
      reinterpret_cast<char*>(std::remove_const_t<decltype(ptr)>(ptr)) - \
      offsetof(type, member))

template <typename T, typename M>
T* get_struct_ptr(M* member_ptr, M T::* member) {
  auto offset =
      reinterpret_cast<std::uintptr_t>(&(reinterpret_cast<T*>(0)->*member));
  return reinterpret_cast<T*>(reinterpret_cast<char*>(member_ptr) - offset);
}

#else

#define container_of(ptr, type, member)                 \
  ({                                                    \
    const __typeof__(((type*)0)->member)* __mptr = ptr; \
    (type*)((char*)__mptr - offsetof(type, member));    \
  })

#endif

#endif  // __COMMON_MEM_H__
