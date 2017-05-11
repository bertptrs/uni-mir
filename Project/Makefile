CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -O2 -g -flto
LDLIBS=-lcurl -lssl -lcrypto

DIRS=data

all: download indexer

clean:
	$(RM) *.o spider
	$(RM) -r $(DIRS)

download: download.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

indexer: indexer.cpp jsoncpp.o Webclient.o PackageCollection.o RepositoryHelper.o | $(DIRS)

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@
