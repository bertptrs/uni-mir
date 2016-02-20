all: Journal.pdf
	$(MAKE) -C sift $@
	$(MAKE) -C spider $@


clean:
	latexmk -c
	$(MAKE) -C sift $@
	$(MAKE) -C spider $@

Journal.pdf: Journal.tex
	latexmk -xelatex $<
