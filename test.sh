#! /usr/bin/env bash
printf 'Test Case => '$1':\n'
printf '    Used: %s ms\n' `(/usr/bin/time -f '%e' bash run.sh $1 > /dev/null) 2>&1`
printf '    Cost: %s\n' `bash check.sh $1`
