all: player.o card.o violetCards.o
	gcc *.o main.c -o main

player.o:
	gcc -c player.c

card.o:
	gcc -c card.c
	
violetCards.o:
	gcc -c violetCards.c

clean:
	rm -f *.o main
