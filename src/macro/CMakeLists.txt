cmake_minimum_required(VERSION 3.5.0)
project(cmacro)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/a.c ${PROJECT_SOURCE_DIR}/main.c)
target_compile_options(${PROJECT_NAME}_test PRIVATE -fno-common)

