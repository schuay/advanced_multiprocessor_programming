set terminal postscript eps enhanced size 13cm,6cm color font 'Helvetica,15'
set output '099contains_005add.eps'
set xrange [1:48]
set nokey
set key top left
set xlabel 'Number of cores [byte]'
set ylabel 'Time [s]'
set logscale x 2
#set logscale y

plot \
'../data/2_globallock.dat' using 1:2 title 'GlobalLock' with linespoints,\
'../data/2_striped.dat' using 1:2 title 'Striped Cuckkoo' with linespoints,\
'../data/2_refined.dat' using 1:2 title 'Refined Cuckoo' with linespoints;
set output
