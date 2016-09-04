
main.pdf: main.tex
	pdflatex main.tex
	mv ../latex/main.pdf ../

PDF: main.pdf
	xpdf main.pdf

clean:
	rm -rf *.out *.aux *.toc *.log
