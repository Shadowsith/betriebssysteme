#!/bin/bash
# noramalise mean values to computation time for one thread
#

rm -f out/mean.norm.dat
awk '
    BEGIN{
        total = 0.0;
        first = -1;
    }
    {
        if (first == -1) {
            print $1, $2, 1.0;
            first = $2;
        } else {
            print $1, $2, $2 / first;
        }
    }' out/mean.dat > out/mean.norm.dat

