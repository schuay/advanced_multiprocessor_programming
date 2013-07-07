PROJECT = protocol

protocol:
	cd ../data/ && ./prep.sh && cd ../protocol/
	./gp.sh
	pdflatex $(PROJECT)
	pdflatex $(PROJECT)

clean:
	rm -f *.aux $(PROJECT).bbl $(PROJECT).blg $(PROJECT).log $(PROJECT).dvi \
		$(PROJECT).toc $(PROJECT).pdf $(PROJECT).ps $(PROJECT).out *.eps *.pdf

