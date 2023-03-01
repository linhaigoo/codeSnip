cmake_minimum_required(VERSION 3.5.0)
project(cpp_stl)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME}_set_compare_test ${PROJECT_SOURCE_DIR}/set_compare.cpp)
add_executable(${PROJECT_NAME}_new_delete_test ${PROJECT_SOURCE_DIR}/new_delete.cpp)
add_executable(${PROJECT_NAME}_remove_erase_test ${PROJECT_SOURCE_DIR}/remove_erase.cpp)
add_executable(${PROJECT_NAME}_insert_test ${PROJECT_SOURCE_DIR}/insert.cpp)
add_executable(${PROJECT_NAME}_reduceMapAcc_test ${PROJECT_SOURCE_DIR}/reduceMapAcc.cpp)
