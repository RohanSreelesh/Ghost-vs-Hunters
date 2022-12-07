a5:	building.o evidence.o ghost.o hunter.o main.o room.o
	gcc -Wall -o a5 building.o evidence.o ghost.o hunter.o main.o room.o -g

building.o:	building.c defs.h
	gcc -c building.c -g

evidence.o:	evidence.c defs.h
	gcc -c evidence.c -g

ghost.o:	ghost.c defs.h
	gcc -c ghost.c -g

hunter.o:	hunter.c defs.h
	gcc -c hunter.c -g

main.o:	main.c defs.h
	gcc -c main.c -g

room.o:	room.c defs.h
	gcc -c room.c -g

clean:
	rm -f building.o evidence.o ghost.o hunter.o main.o room.o a5
