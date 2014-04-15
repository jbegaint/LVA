CC = `which gcc`
LD = `which gcc`
CXX = `which g++`
RM = `which rm`

BBBIOlib_DIR = ./BBBIOlib
BUILD_DIR = ./build
DEMO_DIR = ./c_gpio
LVA_DIR = ./LVA
TESTS_DIR = ./tests

LVA_LIB = liblva.so

INCS = -I$(LVA_DIR) -I/usr/include/ni

CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=gnu99 $(INCS) -O3 -fPIC
CFLAGS_GTK =`pkg-config gtk+-3.0 --cflags` 

# CFLAGS_RLS = $(CFLAGS) -O3
# CFLAGS_DBG = $(CFLAGS) -g -O0

LDFLAGS = -pthread -lOpenNI -lstdc++ -L$(BBBIOlib_DIR) -lBBBio 
LDFLAGS_LVA = -L$(BUILD_DIR) -llva -Wl,-rpath $(BUILD_DIR)
LDFLAGS_GTK = `pkg-config gtk+-3.0 --libs` `pkg-config gmodule-2.0 --libs`

