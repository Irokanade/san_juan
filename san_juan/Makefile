all: player.o card.o
	gcc *.o main.c -o main

player.o:
	gcc -c player.c

card.o:
	gcc -c card.c

clean:
	rm -f *.o main
