CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -O2 -g -flto
LDLIBS=-lcurl -lssl -lcrypto

DIRS=data/repository \
	data/titleindex \
	data/linkindex \
	data/webindex \
	data/pageindex \
	data/imageindex

all: spider

clean:
	$(RM) webspider
	$(RM) webquery
	$(RM) *.o
	$(RM) -r $(DIRS)
	latexmk -c

spider: spider.cpp jsoncpp.o | $(DIRS)

webspider: webspider.cpp htmlstreamparser.o Webclient.o Scraper.o LinkHelper.o Crawler.o RobotsHandler.o IndexHelper.o json| $(DIRS)
	$(CXX) -o $@ $(CXXFLAGS) $^ -lcurl -lssl -lcrypto

webquery: webquery.cpp IndexHelper.o | $(DIRS)
	$(CXX) -o $@ $(CXXFLAGS) $^ -lssl -lcrypto

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@
