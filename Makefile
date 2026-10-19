CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: AlgoritmoGrafo

AlgoritmoGrafo: AlgoritmoGrafo.c Grafo.c Grafo.h
	$(CC) $(CFLAGS) -DGRAFO_SEM_MAIN AlgoritmoGrafo.c Grafo.c -o AlgoritmoGrafo

clean:
	$(RM) AlgoritmoGrafo
