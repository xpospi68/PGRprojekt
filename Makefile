##################################################
# nastaveni
CFLAGS_COMMON=-pipe -Wall -ggdb
CC=gcc
CXX=g++

##################################################
# nastaveni knihoven s OpenGL
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

CXXFLAGS=-std=c++11 $(CFLAGS)

##################################################
# obecne cile

PROJ=proj.exe 

ifdef USE_GLEE
	DEPS=pgr.o GLee.o
else
	DEPS=pgr.o
endif

.PHONY: all proj clean

all: proj

proj: $(PROJ)

clean:
	$(RM) $(PROJ) $(DEPS)

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

