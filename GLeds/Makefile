TARGET = GLeds
CC = gcc
CXX = g++
INCDIR = ../LVA/

SRC := main.c ../LVA/patterns.c ../LVA/matrix.c ../LVA/pgm_utils.c ../LVA/utils.c
SRCXX := ../LVA/oni_utils.cpp

OBJ := $(SRC:%.c=%.o)
OBJXX := $(SRCXX:%.cpp=%.o)

CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic -Wno-unused-parameter -g
CFLAGS += `pkg-config gtk+-3.0 --cflags`
CFLAGS += -I$(INCDIR) -I/usr/include/ni

LDFLAGS = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`
LDFLAGS += -lOpenNI -lstdc++ -pthread

all: $(OBJ) $(OBJXX)
	$(CC) $^ $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $< $(CFLAGS) -I$(dir $<) -c -o $@

%.o: %.cpp
	$(CXX) $< $(CFLAGS) -I$(dir $<) -c -o $@ 

.PHONY: clean

clean:
	rm -f $(OBJ) $(OBJXX)
