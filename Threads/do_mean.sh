#!/bin/bash
# Compute mean values of data files and collect them in one file
#

rm -f out/mean.dat
for f in `ls out/parsum*.dat`; do
    # extract the number of threads
    post=${f#out/parsum.*}
    export threadnum=${post%*.dat}
 
    # Call awk to compute the mean in file $f
    # Append threadnum and mean to file out/mean.dat
    awk '
        {total+=$1;count++;}
        END{
            print ENVIRON["threadnum"], total/count;
        }' $f >> out/mean.dat

done
