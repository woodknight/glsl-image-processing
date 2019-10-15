cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

project(glsl_image)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin)

add_subdirectory(common)
add_subdirectory(overlay)
add_subdirectory(yuv2rgb)
add_subdirectory(single_image)