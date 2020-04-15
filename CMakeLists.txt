cmake_minimum_required(VERSION 3.12.4)

project(hortd VERSION 0.1.0)

enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmarks)
