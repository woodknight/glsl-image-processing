set(SRC main.cpp glad.c)
set(NAME overlay)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_executable(${NAME} ${SRC})
target_include_directories(${NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${NAME} glfw GL dl)

