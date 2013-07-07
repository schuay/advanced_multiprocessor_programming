rm 1_*.dat
rm 2_*.dat
for f in *.dat
do
   sed -e '/test/d' -n -e 's/.*[\ \t]\(0.5	0.25\)[\ \t]0[\ \t]\([0-9]*[\ \t][0-9.]*\)[\ \t]*$/\2/p' $f | sort -g -k1 -k2 | sort -gu -k1 >1_$f
   sed -e '/test/d' -n -e 's/.*[\ \t]\(0.99	0.005\)[\ \t]0[\ \t]\([0-9]*[\ \t][0-9.]*\)[\ \t]*$/\2/p' $f | sort -g -k1 -k2 | sort -gu -k1 >2_$f
done
