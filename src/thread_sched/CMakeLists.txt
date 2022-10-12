cmake_minimum_required(VERSION 3.5.0)
project(thread_sched)
find_package(Threads REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/thread_sched.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE Threads::Threads)

