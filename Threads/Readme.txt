Howto generate statistics

1) build

make clean; make

2) Run test program parsum for various number of threads

./do_runs.sh

3) Create data file with mean values

./do_mean.sh

4) Normalize mean values; base is computation time using one thread.

./do_norm.sh

5) Inspect data with gnuplot

gnuplot -persist histogram.gp

6) Generate png output

gnuplot -e 'set terminal png size 1024,768' histogram.gp > parsum.png

