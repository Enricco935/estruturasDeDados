CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: main

main: main.c Grafo.c Grafo.h
	$(CC) $(CFLAGS) -DGRAFO_SEM_MAIN main.c Grafo.c -o main

clean:
	$(RM) main
