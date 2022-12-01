a5:	building.o evidence.o ghost.o hunter.o main.o room.o
	gcc -Wall -o a5 building.o evidence.o ghost.o hunter.o main.o room.o -g

building.o:	building.c defs.h
	gcc -c building.c

evidence.o:	evidence.c defs.h
	gcc -c evidence.c

ghost.o:	ghost.c defs.h
	gcc -c ghost.c

hunter.o:	hunter.c defs.h
	gcc -c hunter.c

main.o:	main.c defs.h
	gcc -c main.c

room.o:	room.c defs.h
	gcc -c room.c

clean:
	rm -f building.o evidence.o ghost.o hunter.o main.o room.o
