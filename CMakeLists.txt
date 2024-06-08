cmake_minimum_required(VERSION 3.10)
project(mprpc)

# 设置要求的 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGES} -g)

# 配置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置项目库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 头文件搜索路径 -I
# include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/example)
# -I/usr/local/include/google/protobuf  -L/usr/local/lib
include_directories(/usr/local/include/google/protobuf)
include_directories(/usr/local/include/zookeeper)
include_directories(/usr/local/include/) 

# 设置项目库文件搜索路径 -L  protobuf zookeeper的连接库/usr/local/lib
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(/usr/local/lib)
link_directories(/usr/lib)

# 列出所有需要编译的源文件
file(GLOB SRC_LIST
    src/*.cc
)
add_library(mprpc SHARED ${SRC_LIST})
target_link_libraries(mprpc tinymuduo pthread zookeeper_mt)

add_subdirectory(example)