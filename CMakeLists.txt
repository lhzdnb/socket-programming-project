cmake_minimum_required(VERSION 3.26)
project(code)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

link_libraries(ws2_32 wsock32)

# 遍历项目根目录下所有的 .cpp 文件
file (GLOB files *.cpp)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})
    message (\ \ \ \ --\ ./${exe}.cpp\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()

