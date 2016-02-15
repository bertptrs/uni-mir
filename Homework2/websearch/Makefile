BINARY=htmlprocess
CFLAGS=-Wall -Wextra -pedantic

all: htmlprocess showlinks

htmlprocess: htmlprocess.c htmlstreamparser.o
	cc -o $@ $^ -lcurl

showlinks: showlinks.c htmlstreamparser.o
	cc -o $@ $^ -lcurl

clean:
	$(RM) *.o
	$(RM) htmlprocess

