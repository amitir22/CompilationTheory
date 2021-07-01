#!/bin/bash
make -f makefile
./hw5 < staff_tests/t01.in > staff_tests/t01.llvm
lli staff_tests/t01.llvm > staff_tests/t01.res
diff staff_tests/t01.res staff_tests/t01.out
./hw5 < staff_tests/t02.in > staff_tests/t02.llvm
lli staff_tests/t02.llvm > staff_tests/t02.res
diff staff_tests/t02.res staff_tests/t02.out
./hw5 < staff_tests/t03.in > staff_tests/t03.llvm
lli staff_tests/t03.llvm > staff_tests/t03.res
diff staff_tests/t03.res staff_tests/t03.out
