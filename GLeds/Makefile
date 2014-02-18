TARGET = GLeds

SRC := main.c
OBJ := $(SRC:%.c=%.o)

CFLAGS = -I$(INC_DIR) -Wall -Wextra -Wshadow -std=c99 -pedantic
CFLAGS += `pkg-config gtk+-3.0 --cflags`

LDFLAGS = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`

FILES = main.o

all: $(OBJ)
	gcc $^ $(LDFLAGS) -o $(TARGET)

%.o: %.c
	gcc $< $(CFLAGS) -I$(dir $<) -c -o $@

.PHONY: clean

clean:
	rm -f $(OBJ)
