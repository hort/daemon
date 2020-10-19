cmake_minimum_required(VERSION 3.12.4)

project(hortd VERSION 0.1.0)

list(APPEND CMAKE_PREFIX_PATH "/opt/local/lib/cmake")

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS_DEBUG          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")


add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmarks)
