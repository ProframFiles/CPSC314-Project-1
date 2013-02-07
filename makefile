IDIR =.
CC=g++
CFLAGS=-I$(IDIR) -O1
CPPFLAGS=-I$(IDIR) -O1 -std=c++0x

ODIR=obj
LDIR =../lib

LIBS=-lGLU -lglut -lGL

_DEPS = cs314_math.hpp cs314_util.hpp scene_graph.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = stb_image.o akj_ogl.o cs314_util.o rabbit.o rabbit_animations.o p1.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

p1_linux: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)



.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
