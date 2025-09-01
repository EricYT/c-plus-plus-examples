# 最低CMake版本要求
cmake_minimum_required(VERSION 4.0)

# 项目名称(可选，仅用于标识)
project(cpp_examples LANGUAGES CXX)

# 设置C++标准(根据需要选择，如c++11/c++17/c++20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器扩展(如g++的gnu++17)

# 加载通用编译选项(可选，见下方cmake/CompilerOptions.cmake)
include(cmake/CompilerOptions.cmake)

# common options
option(ENABLE_DEBUG "Enable DEBUG verbose message" ON)

if(ENABLE_DEBUG)
  add_compile_definitions(ENABLE_DEBUG)
endif()

# 添加公共工具库(如果有common目录)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/common/CMakeLists.txt)
    add_subdirectory(common)
endif()

# 自动添加所有示例子目录
# 遍历当前目录下所有包含CMakeLists.txt的子目录
file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(DIR ${EXAMPLE_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DIR} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/CMakeLists.txt)
        # 排除common和build目录
        if(NOT DIR STREQUAL "common" AND NOT DIR STREQUAL "build" AND NOT DIR STREQUAL "cmake")
            add_subdirectory(${DIR})
            message(STATUS "Added example: ${DIR}")
        endif()
    endif()
endforeach()
