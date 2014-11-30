##################################################
CFLAGS_COMMON=-pipe -Wall -ggdb
CC=gcc
CXX=g++

##################################################
# nastavenie kniznic s OpenGL
ifeq ($(OS), Windows_NT)
	CFLAGS=$(CFLAGS_COMMON) -Wl,--enable-auto-import
	LIBS=-lopengl32 -lglew32 -lSDL
	RM=del
else
	USE_GLEE=1
	CFLAGS=$(CFLAGS_COMMON) `sdl-config --cflags` -I. -IGL -DUSE_GLEE
	LIBS=-L/usr/lib64 `sdl-config --libs` -lGL
	RM=rm -f
endif

#CFLAGS = `freetype-config --cflags` -IGlew
#-DUSE_GLEE
#LIBS=`freetype-config --libs` -lGLEW

CXXFLAGS=-std=c++11 $(CFLAGS)

##################################################
PROJ=proj.exe
LOGIN=xsokov00

ifdef USE_GLEE
	DEPS=pgr.o GLee.o
else
	DEPS=pgr.o
endif

.PHONY: all proj clean doc run pack

all: proj

doc:
	$(MAKE) -C doc
	cp doc/*.pdf PGR-doc-$(LOGIN).pdf
	$(MAKE) clean -C doc

proj: $(PROJ)

run: $(PROJ)
	./$(PROJ)

pack: clean PGR-doc-$(LOGIN).pdf
	cd ..; zip $(LOGIN)/$(LOGIN).zip -r $(LOGIN) -x \*GL/\* \*glm/\* \*doc/\* \*GLee.c \*proj/Debug/\* \*proj/Release/\* \*proj/proj/\*

clean:
	$(RM) $(PROJ) $(DEPS) $(LOGIN).zip *~


##################################################
# pravidla

%.exe: %.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.exe: %.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

proj.exe: proj.cpp map.hpp $(DEPS) 
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

##################################################
# zavislosti

pgr.o: pgr.cpp pgr.h
#proj.o: proj.cpp $(DEPS) 

