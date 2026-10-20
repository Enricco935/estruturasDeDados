CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: AlgoritmoGrafo main

AlgoritmoGrafo: AlgoritmoGrafo.c Grafo.c Grafo.h
	$(CC) $(CFLAGS) -DGRAFO_SEM_MAIN AlgoritmoGrafo.c Grafo.c -o AlgoritmoGrafo

main: main.c Fila.c Fila.h Lista.c Lista.h Pilha.c Pilha.h Grafo.c Grafo.h
	$(CC) $(CFLAGS) -DGRAFO_SEM_MAIN main.c Fila.c Lista.c Pilha.c Grafo.c -o main

clean:
	$(RM) AlgoritmoGrafo main
