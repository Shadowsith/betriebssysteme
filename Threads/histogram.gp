# Visualization of data in file
#   out/mean.dat
# with gnuplot
#
# 2012-04-30    fabr
#

reset

set title "Laufzeit abhängig von Anzahl der Threads"
set xlabel "Anzahl der Threads"
set ylabel "Normiert auf\nLaufzeit mit nur einen Thread"
set auto x
set yrange [0:1.05]
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
#set xtic rotate by -45
#set bmargin 5 

plot 'out/mean.norm.dat' using 3:xtic(1)

