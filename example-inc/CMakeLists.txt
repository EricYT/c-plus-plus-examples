# 定义可执行文件名称(建议与目录名一致)
set(EXAMPLE_NAME example-inc)

# 添加可执行文件
add_executable(${EXAMPLE_NAME}
    main.cc  # 示例的主文件
)

# 如果需要链接公共库
if(TARGET common)
    target_link_libraries(${EXAMPLE_NAME} PRIVATE common)
endif()

# (可选)设置输出目录，所有示例可执行文件放在同一目录
set_target_properties(${EXAMPLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin  # 可执行文件输出到build/bin
)
