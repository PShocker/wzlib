cmake_minimum_required(VERSION 3.17)
project(wzlibcpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(zlib)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/zlib")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/AES")

include_directories("C:/SDL3-3.4.0/include")
link_directories("C:/SDL3-3.4.0/lib/x64")

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB AES_SOURCE_FILES AES/AES.cpp)

add_library(wzlib ${SOURCE_FILES} ${AES_SOURCE_FILES})
target_link_libraries(wzlib zlibstatic)

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