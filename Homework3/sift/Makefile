CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -g -std=c++11

LIBRARIES=-ljpeg -lm

OBJECTS=alloc.o \
		error.o \
		kdtree.o \
		sift.o \
		xform.o \
		cv.o \
		imgfeatures.o \
		minpq.o \
		smooth.o \
		utils.o

all: getsift similarimage

clean:
	$(RM) getsift
	$(RM) similarimage
	$(RM) *.o

similarimage: similarimage.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBRARIES)

getsift: test.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBRARIES)

