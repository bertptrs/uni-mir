CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -O2 -g -flto
LDLIBS=-lcurl -lssl -lcrypto

DIRS=data

OBJECTS=\
		jsoncpp.o \
		Webclient.o \
		PackageCollection.o \
		Searcher.o \
		RepositoryHelper.o

all: download indexer search

clean:
	$(RM) *.o spider
	$(RM) -r $(DIRS)

download: download.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

indexer: indexer.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

search: search.cpp Searcher.o jsoncpp.o PackageCollection.o RepositoryHelper.o | $(DIRS)

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@


# Automatic dependency detection
%.d: %.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $@ -MP $<

-include $(OBJECTS:.o=.d)
