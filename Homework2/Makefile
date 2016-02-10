
IMAGERETRIEVALDIR=imageretrieval/Debug

.PHONY: $(IMAGERETRIEVALDIR)/imageretrieval

all: report.pdf \
	ranklist.html

clean:
	$(RM) ranklist.html
	$(RM) report.pdf
	$(MAKE) -C $(IMAGERETRIEVALDIR) clean
	latexmk -c

report.pdf: report.tex
	latexmk -xelatex $<

ranklist.html: $(IMAGERETRIEVALDIR)/imageretrieval
	$< ./$(IMAGERETRIEVALDIR)/myimages/index1.jpeg ./$(IMAGERETRIEVALDIR)/myimages/ $@

$(IMAGERETRIEVALDIR)/imageretrieval:
	$(MAKE) -C $(dir $@) $(notdir $@)
