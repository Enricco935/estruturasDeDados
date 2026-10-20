CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic

all: main

main: main.c AlgoritmoGrafo.c AlgoritmoGrafo.h Fila.c Fila.h Lista.c Lista.h Pilha.c Pilha.h Grafo.c Grafo.h
	$(CC) $(CFLAGS) -DGRAFO_SEM_MAIN main.c AlgoritmoGrafo.c Fila.c Lista.c Pilha.c Grafo.c -o main

clean:
	$(RM) main
