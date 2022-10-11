cmake_minimum_required(VERSION 3.5.0)
project(multi_thread)


include_directories(${PROJECT_SOURCE_DIR})
find_package(Threads REQUIRED)
add_library(${PROJECT_NAME}_lib SHARED ${PROJECT_SOURCE_DIR}/multi_thread.cpp)
target_link_libraries(${PROJECT_NAME}_lib PRIVATE Threads::Threads)


add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/multi_thread_test.cpp)
include_directories(${PROJECT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME}_test multi_thread_lib)
