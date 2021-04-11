#!/bin/sh
tests=`ls -d $PWD/build/src/test/*`

for test in $tests; do
  echo "$test"
  $test
done
