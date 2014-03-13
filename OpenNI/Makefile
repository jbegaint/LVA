make: FrameToPgm PgmToLED

FrameToPgm: ./src/traitement.o ./src/main.o
	gcc -Wall -g -o FrameToPgm ./src/traitement.o ./src/main.o

traitement.o: ./src/traitement.c
	gcc -g -o ./src/traitement.o -c ./src/traitement.c -W -Wall

main.o: ./src/main.c 
	gcc -g -o ./src/main.o -c ./src/main.c -W -Wall

PgmToLED: ./src/PgmToLED.o
	gcc -Wall -g -o PgmToLED ./src/PgmToLED.o

PgmToLED.o: ./src/PgmToLED.c
	gcc -g -o ./src/PgmToLED.o -c ./src/PgmToLED.c -W -Wall

clean:
	rm -rf ./src/*.o

