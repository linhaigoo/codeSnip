cmake_minimum_required(VERSION 3.5.0)
project(cpp_template)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_typename_class_test ${PROJECT_SOURCE_DIR}/typename_class.cpp)
add_executable(${PROJECT_NAME}_bloat_test ${PROJECT_SOURCE_DIR}/cpp_bloat.cpp)

