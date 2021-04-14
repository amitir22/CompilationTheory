cmake_minimum_required(VERSION 3.14.5)
project(CompilationTheory)

set(CMAKE_CXX_STANDARD 14)

include_directories("../../../Academic/Technion/semesters/semester 4/COMPI - 236360/My/HW/HW1")

add_executable(CompilationTheory
        HW1/lex.yy.c
        HW1/part_a.cpp
        HW1/tokens.hpp)
