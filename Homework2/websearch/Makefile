BINARY=htmlprocess
CFLAGS=-Wall -Wextra -pedantic -g -std=c99

all: showlinks

showlinks: showlinks.c htmlstreamparser.o
	cc $(CFLAGS) -o $@ $^ -lcurl

clean:
	$(RM) *.o
	$(RM) htmlprocess

