#!/bin/sh
tests=`find build/src/test/ -type f \( ! -iname "*.o" \)`

for test in $tests; do
  echo "$test"
  $test
done
