hello: traitement.o main.o
	gcc -Wall -g -o ConvertFrameToPgm traitement.o main.o

traitement.o: traitement.c
	gcc -g -o traitement.o -c traitement.c -W -Wall

main.o: main.c 
	gcc -g -o main.o -c main.c -W -Wall

clean:
	rm -rf *.o

