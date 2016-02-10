BINARY=htmlprocess
LDFLAGS=-lcurl

all: htmlprocess

htmlprocess: htmlprocess.c htmlstreamparser.o
	cc -o $@ $^ -lcurl

clean:
	$(RM) *.o
	$(RM) htmlprocess

