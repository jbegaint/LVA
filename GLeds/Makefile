TARGET = GLeds
CC = gcc
CXX = g++
INCDIR = ../LVA/

SRC := main.c ../LVA/patterns.c
OBJ := $(SRC:%.c=%.o)

CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic -g -I$(INCDIR) -Wno-unused-parameter
CFLAGS += `pkg-config gtk+-3.0 --cflags`

LDFLAGS = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`
LDFLAGS += -l OpenNI -I /usr/include/ni -lstdc++ -pthread

all: $(OBJ) oni_utils.o
	$(CC) $^ $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $< $(CFLAGS) -I$(dir $<) -c -o $@

oni_utils.o: oni_utils.cpp
	$(CXX) $< -c -o $@ $(CFLAGS) $(LDFLAGS)
.PHONY: clean

clean:
	rm -f $(OBJ)
