cmake_minimum_required(VERSION 3.15)

project(jobber LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER mpic++)

set(CMAKE_CXX_STANDARD 20)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O3 ${WARNING_FLAGS} -fdeclspec -fsanitize=address")

add_subdirectory(src)
