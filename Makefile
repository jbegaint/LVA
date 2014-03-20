# LVA global makefile

include config.mk

SRC := $(shell find $(LVA_DIR) -name '*.c')
OBJC := $(SRC:%.c=%.o)
OBJC_DBG := $(SRC:%.c=%.dbg.o)

SRCXX := $(shell find $(LVA_DIR) -name '*.cpp')
OBJXX := $(SRCXX:%.cpp=%.o)
OBJXX_DBG := $(SRCXX:%.cpp=%.dbg.o)

OBJ = $(OBJC) $(OBJXX)

all: gleds

options:
	@echo build options:
	@echo "CC = ${CC}"
	@echo "CCX = ${CCX}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "CFLAGS_GTK = ${CFLAGS_GTK}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "LDFLAGS_GTK = ${LDFLAGS_GTK}"

gleds: $(OBJ) GLeds/gleds.o
	@mkdir -p $(BUILD_DIR)
	@$(CC) $^ $(LDFLAGS_GTK) -o $(BUILD_DIR)/$@ 

%.o: %.c
	@$(CC) $< $(CFLAGS_GTK) -c -o $@

%.o: %.cpp
	@$(CXX) $< $(CFLAGS) -c -o $@ 

clean:
	@rm -rf $(OBJ) GLeds/main.o
	@rm -rf build

.PHONY: all options clean
