CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -O2 -g

DIRS=data/repository \
	data/titleindex \
	data/linkindex \
	data/webindex \
	data/pageindex \
	data/imageindex

all: webspider webquery Journal.pdf

clean:
	$(RM) webspider
	$(RM) webquery
	$(RM) *.o
	$(RM) -r $(DIRS)
	latexmk -c

webspider: webspider.cpp htmlstreamparser.o Webclient.o Scraper.o LinkHelper.o Crawler.o RobotsHandler.o IndexHelper.o | $(DIRS)
	$(CXX) -o $@ $(CXXFLAGS) $^ -lcurl -lssl -lcrypto

webquery: webquery.cpp IndexHelper.o | $(DIRS)
	$(CXX) -o $@ $(CXXFLAGS) $^ -lssl -lcrypto

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@
