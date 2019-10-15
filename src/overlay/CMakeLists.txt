add_executable(overlay overlay.cpp)
target_include_directories(overlay PRIVATE ${CMAKE_SOURCE_DIR}/common)
target_link_libraries(overlay PRIVATE glad glfw GL dl)

