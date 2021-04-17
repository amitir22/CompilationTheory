cmake_minimum_required(VERSION 3.14.5)
project(CompilationTheory)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_library(Lexer HW1/tokens.hpp HW1/lex.yy.c)

add_executable(CompilationTheory
        HW1/part_b.cpp
        HW1/tokens.hpp)

add_executable(PartA
        HW1/part_a.cpp
        HW1/tokens.hpp)

target_link_libraries(CompilationTheory
        PUBLIC Lexer)

target_link_libraries(PartA
        PUBLIC Lexer)
