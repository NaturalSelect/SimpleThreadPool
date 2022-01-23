cmake_minimum_required(VERSION 3.14.0)

file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}/src" "*.h" "*.hpp" "*.cpp" "*.cc")

add_library(SimpleThreadPool ${src})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SimpleThreadPool PUBLIC Threads::Threads)

if(MSVC)
    target_compile_options(SimpleThreadPool PRIVATE /W4 /WX)
else()
    target_compile_options(SimpleThreadPool PRIVATE -Wall -Wextra -pedantic -Werror)
endif()