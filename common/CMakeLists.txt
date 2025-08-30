# 创建静态库(不链接，仅编译为库文件供其他示例使用)
add_library(common STATIC
    utils.h
    utils.cc
    slice.h
    status.h
    status.cc
)

# 暴露头文件目录(其他示例可通过#include <utils.h>引用)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
