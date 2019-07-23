cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

project(glsl_image)
add_subdirectory(src)