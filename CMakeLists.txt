cmake_minimum_required(VERSION 3.0)
project(Monitor-Inband-Receiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 添加头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR})


# 添加可执行文件Info_Receiver
add_executable(receiver
    Info_Receiver.cpp
)


target_link_libraries(receiver procps z)

install(TARGETS receiver DESTINATION ${CMAKE_INSTALL_BINDIR})
