BINARY=htmlprocess
CFLAGS=-Wall -Wextra -pedantic -g

all: htmlprocess showlinks

htmlprocess: htmlprocess.c htmlstreamparser.o
	cc $(CFLAGS) -o $@ $^ -lcurl

showlinks: showlinks.c htmlstreamparser.o
	cc $(CFLAGS) -o $@ $^ -lcurl

clean:
	$(RM) *.o
	$(RM) htmlprocess

