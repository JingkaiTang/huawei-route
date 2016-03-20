#! /usr/bin/env bash
for arg in $*
do
  printf 'Test Case => '$arg':\n'
  printf '    Used: %s s\n' `(/usr/bin/time -f '%e' bash run.sh $arg > /dev/null) 2>&1`
  printf '    Cost: %s\n' `bash check.sh $arg`
done
