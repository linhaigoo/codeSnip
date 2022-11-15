cmake_minimum_required(VERSION 3.5.0)
project(cpp_static)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_method_static_test ${PROJECT_SOURCE_DIR}/method_static.cpp)
add_executable(${PROJECT_NAME}_object_count_test ${PROJECT_SOURCE_DIR}/object_count.cpp)
