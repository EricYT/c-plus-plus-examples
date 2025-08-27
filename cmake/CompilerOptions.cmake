# 通用编译选项
add_compile_options(
    -Wall            # 启用基本警告
    -Wextra          # 额外警告
    -Wpedantic       # 严格遵循C++标准
    -Werror          # 将警告视为错误(可选，严格模式)
    -O0              # 禁用优化(调试时用，发布时可改为-O2)
    -g               # 生成调试信息
)

# 针对不同编译器的特殊选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC特定选项
    add_compile_options(-Wno-maybe-uninitialized)  # 抑制可能的未初始化警告
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang特定选项
    add_compile_options(-Wno-unused-private-field)  # 抑制未使用私有成员警告
elseif(MSVC)
    # MSVC(Visual Studio)特定选项
    add_compile_options(/W4 /wd4505)  # 警告级别4，忽略未使用函数警告
endif()
