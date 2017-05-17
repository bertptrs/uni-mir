CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -g
LDLIBS=-lcurl -lssl -lcrypto

DIRS=data

OBJECTS=\
		jsoncpp.o \
		Webclient.o \
		PackageCollection.o \
		Searcher.o \
		RepositoryHelper.o

.PHONY: all clean newindex

all: download indexer search

clean:
	$(RM) *.o spider
	$(RM) -r $(DIRS)

newindex: indexer
	$(RM) -r data/1 data/2 data/3 data/4
	./indexer
	./pagerank

download: download.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

indexer: indexer.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

search: search.cpp Searcher.o jsoncpp.o RepositoryHelper.o | $(DIRS)

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@


# Automatic dependency detection
%.d: %.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ -MP $<

-include $(OBJECTS:.o=.d)
