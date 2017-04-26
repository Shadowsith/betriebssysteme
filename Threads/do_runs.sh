#!/bin/bash
#
# Do several runs of summation in loops
#
# Outer loop: number of threads
# Inner loop: number of runs
#

runs=10
maxThreads=8
# for 4GB on 64bit machine
options="-s 536870912"

# for 1GB on 32bit machine
#options="-s 268435456"

# cleanup
rm -rf out
mkdir -p out

n=1
while [ $n -le $maxThreads ]; do
    echo "Doing $runs runs with $n threads"

    i=0
    while [ $i -lt $runs ]; do
        bin/parsum $options -t $n >> out/parsum.${n}.dat
        i=$(($i +1))
    done
    n=$(($n+1))
done
