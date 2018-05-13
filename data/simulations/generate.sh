#!/bin/bash
if [ ! $# -eq 1 ]
then
    echo "Usage: $0 <simulate_executable>" >&2
    exit 1
fi

for mut in {0.1,1}
do
    for m in {3,5,7,9}
    do
        if [ ! -e m${m}_mut${mut} ]
        then
            mkdir m${m}_mut${mut}
        fi
        $1 -kP 2 -k 2 -K 5e4 -D 2e-7 -mut ${mut} -m $m -s 0 -N 10 -P 0.8 -E 0.001 -C 200 -o m${m}_mut${mut}/ -c coloring.txt -p 1
    done
done

