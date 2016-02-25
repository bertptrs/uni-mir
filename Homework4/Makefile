CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g -O2
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -O2 -g

all: webspider

clean:
	$(RM) webspider
	$(RM) *.o

webspider: webspider.cpp htmlstreamparser.o Webclient.o Scraper.o LinkHelper.o Crawler.o
	$(CXX) -o $@ $(CXXFLAGS) $^ -lcurl -lssl

