cmake_minimum_required(VERSION 3.19)
project(HW5_new)

set(CMAKE_CXX_STANDARD 14)

add_executable(HW5_new Semantics.h Semantics.cpp parser.ypp hw3_output.cpp hw3_output.hpp Registers.cpp Registers.h scanner.lex bp.cpp bp.hpp)