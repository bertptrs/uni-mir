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

spider: spider.cpp jsoncpp.o Webclient.o | $(DIRS)

Journal.pdf: Journal.tex
	latexmk -xelatex $<

$(DIRS):
	mkdir -p $@
