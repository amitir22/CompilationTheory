#!/bin/bash

echo "TESTS:"
for num in {1..61}
do
  ./hw3 < "tests/t$num.in" &> "more_tests/t$num.res"
  result="FAILED"

  if [$(diff "tests/t$num.res" "more_tests/t$num.out") != ""]
  then
    result="SUCCESS"
  fi

  echo "TEST $num: $result"

done
echo ''

echo "YO TESTS:"

for num in {1..3}
do
  ./hw3 < "oy_tests/oy$num.in" &> "yo_tests/oy$num.res"

  result="FAILED"

  if [$(diff "oy_tests/oy$num.res" "yo_tests/oy$num.out") != ""]
  then
    result="SUCCESS"
  fi

  echo "TEST YO $num: $result"
done
echo ''
