# This is a simple Makefile for small projects.  When you
# type make at the command prompt, it will compile the code.
# For more depth, see http://www.gnu.org/software/make/manual/make.html

CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g

all: webspider

clean:
	$(RM) webspider
	$(RM) *.o

webspider: webspider.c htmlstreamparser.o scraper.o
	$(CC) -o $@ $(CFLAGS) $^ -lcurl

