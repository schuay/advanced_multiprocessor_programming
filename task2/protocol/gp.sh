rm -f *.pdf
rm -f *.eps

for f in *.gp; do
	gnuplot $f;
done

