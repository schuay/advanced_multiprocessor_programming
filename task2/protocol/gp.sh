rm *.pdf
rm *.eps

for f in *.gp; do
	gnuplot $f;
done

