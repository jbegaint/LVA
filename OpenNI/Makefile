FrameToPgm: traitement.o main.o
	gcc -Wall -g -o FrameToPgm traitement.o main.o

traitement.o: traitement.c
	gcc -g -o traitement.o -c traitement.c -W -Wall

main.o: main.c 
	gcc -g -o main.o -c main.c -W -Wall

PgmToLED: PgmToLED.o
	gcc -Wall -g -o PgmToLED PgmToLED.o

PgmToLED.o: PgmToLED.c
	gcc -g -o PgmToLED.o -c PgmToLED.c -W -Wall

clean:
	rm -rf *.o

