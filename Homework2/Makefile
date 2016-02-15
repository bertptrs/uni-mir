
IMAGERETRIEVALDIR=imageretrieval/Debug

.PHONY: $(IMAGERETRIEVALDIR)/imageretrieval

all: Journal.pdf ranklist.html
	$(MAKE) -C websearch

clean:
	$(RM) ranklist.html
	$(RM) Journal.pdf
	$(MAKE) -C $(IMAGERETRIEVALDIR) clean
	$(MAKE) -C websearch clean
	latexmk -c

Journal.pdf: Journal.tex
	latexmk -xelatex $<

ranklist.html: $(IMAGERETRIEVALDIR)/imageretrieval
	$< ./$(IMAGERETRIEVALDIR)/myimages/index1.jpeg ./$(IMAGERETRIEVALDIR)/myimages/ $@

$(IMAGERETRIEVALDIR)/imageretrieval:
	$(MAKE) -C $(dir $@) $(notdir $@)
