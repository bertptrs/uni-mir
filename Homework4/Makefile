CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -O2
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -O2 -g

DIRS=data/repository \
	data/titleindex \
	data/linkindex \
	data/webindex \
	data/pageindex

all: webspider

clean:
	$(RM) webspider
	$(RM) *.o
	$(RM) -r $(DIRS)

webspider: webspider.cpp htmlstreamparser.o Webclient.o Scraper.o LinkHelper.o Crawler.o RobotsHandler.o IndexHelper.o | $(DIRS)
	$(CXX) -o $@ $(CXXFLAGS) $^ -lcurl -lssl -lcrypto

$(DIRS):
	mkdir -p $@
