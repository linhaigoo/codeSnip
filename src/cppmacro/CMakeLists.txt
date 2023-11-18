cmake_minimum_required(VERSION 3.5.0)
project(cppmacro)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/a.cpp ${PROJECT_SOURCE_DIR}/main.cpp)
target_compile_options(${PROJECT_NAME}_test PRIVATE -fno-common)

