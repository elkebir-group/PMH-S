#!/bin/bash
if [ ! $# -eq 1 ]
then
    echo "Usage: $0 <DP_executable>" >&2
    exit 1
fi

for mut in {0.1,1}
do
    for m in {3,5,7,9}
    do
        for f in ../data/simulations/m${m}_mut${mut}/T_seed*.tree
        do
          echo "$1 -p P $f $(dirname $f)/$(basename $f .tree).labeling > m${m}_mut${mut}_$(basename $f .tree).DP.out"
        done
    done
done
