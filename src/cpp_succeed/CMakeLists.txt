cmake_minimum_required(VERSION 3.5.0)
project(cpp_succeed)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_new_test ${PROJECT_SOURCE_DIR}/cpp_new_delete.cpp)
add_executable(${PROJECT_NAME}_virtual_test ${PROJECT_SOURCE_DIR}/cpp_virtual_call.cpp)
add_executable(${PROJECT_NAME}_multi_test ${PROJECT_SOURCE_DIR}/cpp_multi.cpp)

