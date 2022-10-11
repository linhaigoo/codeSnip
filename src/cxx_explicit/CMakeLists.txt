cmake_minimum_required(VERSION 3.5.0)
project(cxx_explicit)

add_executable(${PROJECT_NAME}_explicit ${PROJECT_SOURCE_DIR}/explicit.cpp)
include_directories(${PROJECT_SOURCE_DIR})
