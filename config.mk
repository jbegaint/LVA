CC = gcc
CXX = g++

LVA_DIR = ./LVA
BBBIOlib_DIR = ./BBBIOlib
BUILD_DIR = ./build
DEMO_DIR = ./c_gpio

INCS = -I$(LVA_DIR) -I/usr/include/ni

CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=gnu99 $(INCS)

CFLAGS_RLS = $(CFLAGS) -O3
CFLAGS_DBG = $(CFLAGS) -g -O0

CFLAGS_GTK = $(CFLAGS) `pkg-config gtk+-3.0 --cflags` 

LDFLAGS = -pthread -lOpenNI -lstdc++ -L$(BBBIOlib_DIR) -lBBBio
LDFLAGS_GTK = `pkg-config gtk+-3.0 --libs` `pkg-config gmodule-2.0 --libs`