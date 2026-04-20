cmake_minimum_required(VERSION 3.17)
project(wzlibcpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # 去掉调试符号
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
        # 启用lto -flto=auto
        # 对本机cpu生成特定优化代码,可能存在兼容性问题
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=auto")
    endif()
endif()

add_subdirectory(zlib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/zlib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/wz")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/AES")

include_directories("C:/SDL3-3.4.0/include")
link_directories("C:/SDL3-3.4.0/lib/x64")

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB AES_SOURCE_FILES AES/AES.cpp)

add_library(wzlib STATIC 
    ${SOURCE_FILES} 
    ${AES_SOURCE_FILES}
    $<TARGET_OBJECTS:zlibstatic>  # 直接使用 zlibstatic 的对象文件
)

if(WIN32)
        project(wzlibtest)

        add_executable(wzlibtest main/main.cpp ${SOURCE_FILES})
        target_link_libraries(wzlibtest PRIVATE SDL3)

        target_link_libraries(
                wzlibtest
                PRIVATE
                wzlib
        )
endif()