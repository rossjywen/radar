# 指定CMake 最低版本
cmake_minimum_required(VERSION 3.14)

# 定義使用的語言
project(Radar, LANGUAGES CXX)

# 設置C++標準不能低於14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 設置不同平臺的編譯器，目前只支持Ubuntu和MacOS
if(APPLE)
    message(STATUS "[Ross]: in MacOS, try to use clang++")
    add_compile_options(-Wall -Wextra -O2)
elseif(UNIX)
    message(STATUS "[Ross]: in Ubuntu, try to use g++")
    add_compile_options(-Wall -Wextra -O2)
endif()


# 添加可執行文件
add_executable(radar_test example/raw_data.cpp)




