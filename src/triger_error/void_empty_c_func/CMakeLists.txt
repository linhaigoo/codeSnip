cmake_minimum_required(VERSION 3.5.0)
project(void_empty_c_func)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/main.c)