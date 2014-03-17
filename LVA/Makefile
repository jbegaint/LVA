CC = gcc
CXX = g++

LVA_DIR = ../LVA
INCDIR = $(LVA_DIR)

CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic -g -Wno-unused-parameter
CFLAGS += -I$(INCDIR) -I/usr/include/ni

LIB_PATH = ../BBBIOlib/BBBio_lib/
LDFLAGS = -L $(LIB_PATH) -lBBBio -pthread -lOpenNI -lstdc++

SRC := $(shell find $(LVA_DIR) -name '*.c')
OBJ := $(SRC:%.c=%.o)

SRCXX := $(shell find $(LVA_DIR) -name '*.cpp')
OBJXX := $(SRCXX:%.cpp=%.o)

DEPS = $(OBJ) $(OBJXX)

pgm_test: $(DEPS)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $< $(CFLAGS) -I$(dir $<) -c -o $@

%.o: %.cpp
	$(CXX) $< $(CFLAGS) -I$(dir $<) -c -o $@ 


.PHONY: clean doxy

clean:
	@rm -f *.o 
	@rm -rf ./doc
	
doxy: 
	@doxygen
