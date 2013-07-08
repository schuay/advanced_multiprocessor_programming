set terminal postscript eps enhanced size 13cm,6cm color font 'Helvetica,15'
set output 'refined.eps'
set xrange [1:48]
set nokey
set key top left
set xlabel 'Number of cores [byte]'
set ylabel 'Time [s]'
set logscale x 2
#set logscale y

plot \
'../data/1_refined.dat' using 1:2 title 'Refined (final implementation): 50% contains, 25% add operations' with linespoints,\
'../data/1_refined_refCounting.dat' using 1:2 title 'Refined (reference counting): 99% contains, 0.5% add operations' with linespoints,\
'../data/2_refined.dat' using 1:2 title 'Refined (final implementation): 99% contains, 0.5% add operations' with linespoints,\
'../data/2_refined_refCounting.dat' using 1:2 title 'Refined (reference counting): 50% contains, 25% add operations' with linespoints;

