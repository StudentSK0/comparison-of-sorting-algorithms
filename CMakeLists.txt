cmake_minimum_required(VERSION 3.10)
project(string_sort_analysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(string_sort_analysis
    src/main.cpp
    src/string_generator.cpp
    src/string_sort_algorithms.cpp
    src/string_sort_tester.cpp
)

target_include_directories(string_sort_analysis PRIVATE include)
