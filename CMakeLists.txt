# 指定CMake 最低版本
cmake_minimum_required(VERSION 3.14)

# 定義使用的語言
project(Radar, LANGUAGES CXX)

# 設置C++標準不能低於14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 設置不同平臺的編譯器，目前只支持Ubuntu和MacOS
# 編譯器會自動匹配，Ubuntu 使用 g++；MacOS 使用 clang++，這是和Makefile的一個區別
# 如果需要手動的指定編譯器，可以在命令行中使用: CXX=編譯器名稱 cmake ..
if(APPLE)
    message(STATUS "[Ross]: in MacOS, try to use clang++")
    add_compile_options(-Wall -Wextra -O2)
elseif(UNIX)
    message(STATUS "[Ross]: in Ubuntu, try to use g++")
    add_compile_options(-Wall -Wextra -O2)
endif()


# 添加所有的源文件
set(RADAR_SOURCES
    src/platform/RadarDeviceCommon.cpp
    src/platform/BoardInstance.cpp
    src/strata/BridgeSerial.cpp
    src/strata/BridgeProtocol.cpp
    src/strata/SerialPort.cpp
    example/raw_data.cpp
    )


# 添加可執行文件
# 這句代碼就完全等同於Makefile中的 $CXX example/raw_data.cpp -o radar_test
add_executable(radar_test ${RADAR_SOURCES})


# 添加头文件搜索路径
message(STATUS "[Ross]: include directory: ${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(radar_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)



# 用下面這兩句來編譯
# cmake -B build
# cmake --build build


