#!/bin/bash

dos2unix -q tests/*.in tests/*.out

./part_b.out < tests/tb1.in > tests/tb1.actual
./part_b.out < tests/tb2.in > tests/tb2.actual
./part_b.out < tests/b1.in > tests/b1.actual
./part_b.out < tests/b2.in > tests/b2.actual
./part_b.out < tests/b3.in > tests/b3.actual
./part_b.out < tests/b4.in > tests/b4.actual
./part_b.out < tests/b5.in > tests/b5.actual

diff tests/tb1.out tests/tb1.actual
diff tests/tb2.out tests/tb2.actual
diff tests/b1.out tests/b1.actual
diff tests/b2.out tests/b2.actual
diff tests/b3.out tests/b3.actual
diff tests/b4.out tests/b4.actual
diff tests/b5.out tests/b5.actual
