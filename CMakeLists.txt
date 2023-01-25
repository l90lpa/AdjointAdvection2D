cmake_minimum_required(VERSION 3.10)

project(advection2d)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES "main.cpp")
set(HDR_FILES "third/bitmap_image.hpp" "third/array2d.hpp" "array2d_utils.hpp")

add_executable(advection2d ${SRC_FILES} ${HDR_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} "third")
