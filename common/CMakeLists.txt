# 创建静态库(不链接，仅编译为库文件供其他示例使用)
add_library(common STATIC
    coding.h
    coding.cc
    mutexlock.h
    port_stdcxx.h
    thread_annotations.h
    utils.h
    utils.cc
    slice.h
    status.h
    status.cc
    debug.h
)

# 暴露头文件目录(其他示例可通过#include <utils.h>引用)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
