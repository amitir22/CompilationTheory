cmake_minimum_required(VERSION 3.14.5)
project(CompilationTheory)

set(CMAKE_CXX_STANDARD 11)

include_directories(HW1)

add_executable(CompilationTheory
        HW1/part_b.cpp
        HW1/tokens.hpp)
