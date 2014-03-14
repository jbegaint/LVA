CFLAGS = -Wall -Wextra -Wshadow -std=gnu99 -pedantic

all: FrameToPgm PgmToLED

FrameToPgm: ./src/traitement.o ./src/main.o
	gcc $^ $(LDFLAGS) -o $@

PgmToLED: ./src/PgmToLED.o
	gcc $^ $(LDFLAGS) -o $@

%.o: %.c
	gcc $< $(CFLAGS) -I$(dir $<) -c -o $@

clean:
	rm -rf ./src/*.o