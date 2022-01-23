cmake_minimum_required(VERSION 3.14.0)

file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/test" "*.h" "*.hpp" "*.cpp" "*.cc")

add_executable(Test ${src})

target_link_libraries(Test SimpleThreadPool)