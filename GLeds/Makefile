TARGET = GLeds
CC = gcc
CXX = g++
INCDIR = ../LVA/

SRC := main.c ../LVA/patterns.c ../LVA/matrix.c
SRCXX := ../LVA/oni_utils.cpp

OBJ := $(SRC:%.c=%.o)
OBJXX := $(SRCXX:%.cpp=%.o)

CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic -g -I$(INCDIR) -Wno-unused-parameter
CFLAGS += `pkg-config gtk+-3.0 --cflags`

LDFLAGS = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`
LDFLAGS += -l OpenNI -I /usr/include/ni -lstdc++ -pthread

all: $(OBJ) $(OBJXX)
	$(CC) $^ $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $< $(CFLAGS) -I$(dir $<) -c -o $@

# ../LVA/oni_utils.o: ../LVA/oni_utils.cpp
%.o: %.cpp
	$(CXX) $< -c -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ)
