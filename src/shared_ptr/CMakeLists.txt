cmake_minimum_required(VERSION 3.5.0)
project(shared_ptr)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_test ${PROJECT_SOURCE_DIR}/shared_ptr.cpp)
add_executable(PFMCtestV0 ${PROJECT_SOURCE_DIR}/PFMCtestV0.cpp)
add_executable(PFMCtestV1 ${PROJECT_SOURCE_DIR}/PFMCtestV1.cpp)

