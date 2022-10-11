cmake_minimum_required(VERSION 3.1.0)
project(raii_item)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/raii_item.cpp)


