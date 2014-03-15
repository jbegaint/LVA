TARGET = GLeds

SRC := main.c ../c_gpio/patterns.c
OBJ := $(SRC:%.c=%.o)

CFLAGS = -I$(INC_DIR) -Wall -Wextra -Wshadow -std=gnu99 -pedantic -g
CFLAGS += `pkg-config gtk+-3.0 --cflags`

LDFLAGS = `pkg-config gtk+-3.0 --libs`
LDFLAGS += `pkg-config gmodule-2.0 --libs`

all: $(OBJ)
	gcc $^ $(LDFLAGS) -o $(TARGET)

%.o: %.c
	gcc $< $(CFLAGS) -I$(dir $<) -c -o $@

.PHONY: clean

clean:
	@rm -f $(OBJ)
