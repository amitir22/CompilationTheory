cmake_minimum_required(VERSION 3.14.5)
project(CompilationTheory)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompilationTheory
        HW1/lex.yy.c
        HW1/part_a.cpp
        HW1/part_b.cpp
        HW1/tokens.hpp)
