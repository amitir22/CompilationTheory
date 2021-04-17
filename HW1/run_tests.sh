#!/bin/bash

dos2unix -q tests/*.in tests/*.out

# part a tests
./part_a.out < tests/ta1.in > tests/ta1.actual
./part_a.out < tests/ta2.in > tests/ta2.actual

# part b tests
./part_b.out < tests/tb1.in > tests/tb1.actual
./part_b.out < tests/tb2.in > tests/tb2.actual
./part_b.out < tests/b1.in > tests/b1.actual
./part_b.out < tests/b2.in > tests/b2.actual
./part_b.out < tests/b3.in > tests/b3.actual
./part_b.out < tests/b4.in > tests/b4.actual
./part_b.out < tests/b5.in > tests/b5.actual
./part_b.out < tests/b_external1.in > tests/b_external1.actual
./part_b.out < tests/b_external2.in > tests/b_external2.actual
./part_b.out < tests/b_external3.in > tests/b_external3.actual
./part_b.out < tests/b_external4.in > tests/b_external4.actual
./part_b.out < tests/b_external5.in > tests/b_external5.actual
./part_b.out < tests/b_external6.in > tests/b_external6.actual
./part_b.out < tests/b_external7.in > tests/b_external7.actual

# diffs part a
diff tests/ta1.out tests/ta1.actual
diff tests/ta2.out tests/ta2.actual

# diffs part b
diff tests/tb1.out tests/tb1.actual
diff tests/tb2.out tests/tb2.actual
diff tests/b_external1.out tests/b_external1.actual
diff tests/b_external2.out tests/b_external2.actual
diff tests/b_external3.out tests/b_external3.actual
diff tests/b_external4.out tests/b_external4.actual
diff tests/b_external5.out tests/b_external5.actual
diff tests/b_external6.out tests/b_external6.actual
diff tests/b_external7.out tests/b_external7.actual
