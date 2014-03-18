CC = gcc
CXX = g++

LVA_DIR = ../LVA
INCDIR = $(LVA_DIR)

CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic -g -Wno-unused-parameter
CFLAGS += -I$(INCDIR) -I/usr/include/ni

LIB_PATH = ../BBBIOlib/BBBio_lib/
LDFLAGS = -L $(LIB_PATH) -lBBBio -pthread -lOpenNI -lstdc++

SRC := $(shell find $(LVA_DIR) -name '*.c' ! -name 'pgm_test.c')
OBJ := $(SRC:%.c=%.o)

SRCXX := $(shell find $(LVA_DIR) -name '*.cpp')
OBJXX := $(SRCXX:%.cpp=%.o)

DEPS = $(OBJ) $(OBJXX)

all: demo_pattern_7x5

demo_pattern_7x5: demo_pattern_7x5.o $(DEPS)
	$(CC) $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

demo_matrix: demo_matrix.o $(DEPS)
	$(CC) $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

# tests: tests.o $(DEPS)
	# $(CC) $^ $(LDFLAGS) -o $@

demo_one_row: demo_one_row.o $(DEPS)
	$(CC) $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

demo_cycle: demo_cycle.o $(DEPS)
	$(CC) $^ $(LDFLAGS) -L ${LIB_PATH} -lBBBio -o $@

libBBBio.a: ${LIB_PATH}BBBiolib.c ${LIB_PATH}BBBiolib.h BBBiolib_PWMSS.o BBBiolib_McSPI.o BBBiolib_ADCTSC.o
	$(CC) -c ${LIB_PATH}BBBiolib.c -o ${LIB_PATH}BBBiolib.o
	ar -rs ${LIB_PATH}libBBBio.a ${LIB_PATH}BBBiolib.o ${LIB_PATH}BBBiolib_PWMSS.o ${LIB_PATH}BBBiolib_McSPI.o ${LIB_PATH}BBBiolib_ADCTSC.o
	cp ${LIB_PATH}libBBBio.a ./

BBBiolib_PWMSS.o: ${LIB_PATH}BBBiolib_PWMSS.c ${LIB_PATH}BBBiolib_PWMSS.h
	$(CC) -c ${LIB_PATH}BBBiolib_PWMSS.c -o ${LIB_PATH}BBBiolib_PWMSS.o

BBBiolib_McSPI.o: ${LIB_PATH}BBBiolib_McSPI.c ${LIB_PATH}BBBiolib_PWMSS.h
	$(CC) -c ${LIB_PATH}BBBiolib_McSPI.c -o ${LIB_PATH}BBBiolib_McSPI.o

BBBiolib_ADCTSC.o: ${LIB_PATH}BBBiolib_ADCTSC.c ${LIB_PATH}BBBiolib_ADCTSC.h
	$(CC) -c ${LIB_PATH}BBBiolib_ADCTSC.c -o ${LIB_PATH}BBBiolib_ADCTSC.o

%.o: %.c
	$(CC) $< $(CFLAGS) -I$(dir $<) -c -o $@

%.o: %.cpp
	$(CXX) $< $(CFLAGS) -I$(dir $<) -c -o $@ 

.PHONY: clean doxy

clean:
	@rm -f *.o *.a demo_cycle demo_one_row
	@rm -rf ./doc
	
doxy: 
	@doxygen
