CC = gcc
CXX = g++

LVA_DIR = ./LVA
BBBIOlib_DIR = ./BBBIOlib
BUILD_DIR = ./build

INCS = -I$(LVA_DIR) -I/usr/include/ni

CFLAGS = -Wall -Wextra -Wshadow -pedantic -std=gnu99 $(INCS)

CFLAGS_RLS = $(CFLAGS) -O3
CFLAGS_DBG = $(CFLAGS) -g -O0

LDFLAGS = -L$(BBBIOlib_DIR) -lBBBio -pthread -lOpenNI -lstdc++

CFLAGS_GTK = $(CFLAGS) `pkg-config gtk+-3.0 --cflags` 
LDFLAGS_GTK = $(LDFLAGS) `pkg-config gtk+-3.0 --libs` `pkg-config gmodule-2.0 --libs`
