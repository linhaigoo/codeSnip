cmake_minimum_required(VERSION 3.5.0)
project(promise_future)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/promise_get_future.cpp)


