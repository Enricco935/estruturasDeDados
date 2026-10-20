// Implementação de TADs simples e grafo

// Alunos:
// Enricco Augusto Rodrigues Faria  | RA: 0117036
// João Vitor Da Silva Silverio     | RA: 0117181
// 19/10/2026

// Professor:
// Walace de Almeida Rodrigues


#include <stdio.h>
#include <stdlib.h>
#include "AlgoritmoGrafo.h"
#include "Fila.h"
#include "Grafo.h"
#include "Lista.h"
#include "Pilha.h"

static int comparaInteiros(void *a, void *b) {
    return *(int *)a == *(int *)b;
}

static void testaPilha(void) {
    int primeiro = 10;
    int segundo = 20;
    Pilha *pilha = Pcria();

    Pinsere(pilha, &primeiro);
    Pinsere(pilha, &segundo);

    printf("Empilhei: 10, 20\n");
    printf("Topo: %d\n", *(int *)Pexamina(pilha));
    printf("Desempilhei: %d\n", *(int *)Premove(pilha));
    printf("Tamanho restante: %d\n", Ptamanho(pilha));

    Pdestroi(&pilha);
}

static void testaFila(void) {
    int primeiro = 10;
    int segundo = 20;
    Fila *fila = Fcria();

    Finsere(fila, &primeiro);
    Finsere(fila, &segundo);

    printf("Enfileirei: 10, 20\n");
    printf("Primeiro da fila: %d\n", *(int *)Fexamina(fila));
    printf("Desenfileirei: %d\n", *(int *)Fremove(fila));
    printf("Tamanho restante: %d\n", Ftamanho(fila));

    Fdestroi(&fila);
}

static void testaLista(void) {
    int dez = 10;
    int vinte = 20;
    int quinze = 15;
    Lista *lista = Lcria();

    Linsere(lista, &dez, 0);
    Linsere(lista, &vinte, 1);
    Linsere(lista, &quinze, 1);

    printf("Lista apos inserir 10, 20 e 15 na posicao 1: ");
    for (int i = 0; i < Ltamanho(lista); i++) {
        printf("%s%d", i == 0 ? "" : " -> ",
               *(int *)Lexamina(lista, i));
    }
    printf("\nPosicao do valor 20: %d\n",
           Llocaliza(lista, &vinte, comparaInteiros));

    Ldestroi(&lista);
}

static void testaGrafo(void) {
    printf("\nCriando um grafo nao dirigido com capacidade para "
           "4 vertices e 3 arestas.\n");
    Grafo *grafo = GGcriaGrafo(4, 3);
    if (grafo == NULL) {
        printf("Nao foi possivel criar o grafo.\n");
        return;
    }

    printf("Adicionando os vertices v0, v1, v2 e v3...\n");
    for (int i = 0; i < 4; i++) {
        if (GVcriaVertice(grafo) == NULL) {
            printf("Falha ao adicionar v%d.\n", i);
            GGdestroiGrafo(grafo);
            return;
        }
    }
    printf("Vertices adicionados: %d\n", GInumeroVertices(grafo));

    printf("Adicionando as arestas: v0-v1, v0-v2 e v1-v3...\n");
    const int extremos[][2] = {{0, 1}, {0, 2}, {1, 3}};
    for (int i = 0; i < 3; i++) {
        if (GAcriaAresta(grafo, extremos[i][0], extremos[i][1]) == NULL) {
            printf("Falha ao conectar v%d e v%d.\n",
                   extremos[i][0], extremos[i][1]);
            GGdestroiGrafo(grafo);
            return;
        }
    }

    printf("\nGrafo criado (%d vertices, %d arestas):\n",
           GInumeroVertices(grafo), GInumeroArestas(grafo));
    int aresta = GAprimeiraAresta(grafo);
    while (aresta != -1) {
        printf("  a%d conecta v%d e v%d\n",
               aresta, GValfa(grafo, aresta), GVomega(grafo, aresta));
        aresta = GAproximaAresta(grafo, aresta);
    }

    printf("\nConsultas:\n");
    printf("  v0 existe? %s\n",
           GBexisteIdVertice(grafo, 0) ? "sim" : "nao");
    printf("  Existe aresta entre v0 e v1? %s\n",
           GBexisteAresta(grafo, 0, 1) ? "sim" : "nao");
    printf("  Existe aresta entre v0 e v3? %s\n",
           GBexisteAresta(grafo, 0, 3) ? "sim" : "nao");
    printf("  Grau de v0: %d (conectado a duas arestas)\n",
           GIpegaGrau(grafo, 0));

    printf("\nRepresentacao: v2 -- v0 -- v1 -- v3\n");
    printf("                 a1    a0    a2\n");

    if (GBsalvaGrafoDot(grafo, "grafo_teste.dot")) {
        printf("\nGrafo salvo em 'grafo_teste.dot'. Abra o arquivo "
               "com Graphviz para visualizar.\n");
    } else {
        printf("\nNao foi possivel salvar 'grafo_teste.dot'.\n");
    }

    GGdestroiGrafo(grafo);
    printf("Grafo destruido e memoria liberada.\n");
}

static void executaCaminhoComPesos(void) {
    const int arestas[][2] = {{5, 0}, {5, 3}, {2, 5}, {0, 2}, {0, 1}, {1, 2}, {1, 1}, {4, 3}};
    const float pesos[] = {0, 1, 1, 1, 1, 1, 1, 1, 1};

    printf("\n=== Demonstracao: caminho mais curto ===\n");
    printf("Criando um grafo dirigido com vertices de 0 a 5.\n");
    Grafo *grafo = GGcriaGrafoTipo(6, 8, 1);
    if (grafo == NULL) {
        printf("Nao foi possivel criar o grafo.\n");
        return;
    }

    for (int i = 0; i < 6; i++) {
        if (GVcriaVertice(grafo) == NULL) {
            printf("Nao foi possivel adicionar os vertices.\n");
            GGdestroiGrafo(grafo);
            return;
        }
    }

    printf("Adicionando arestas e seus pesos:\n");
    for (int i = 0; i < 8; i++) {
        if (GAcriaAresta(grafo, arestas[i][0], arestas[i][1]) == NULL) {
            printf("Nao foi possivel adicionar a aresta a%d.\n", i);
            GGdestroiGrafo(grafo);
            return;
        }
        printf("  a%d: %d -> %d, peso %.0f\n",
               i, arestas[i][0], arestas[i][1], pesos[i + 1]);
    }

    printf("Vetor de pesos: (0, 1, 1, 1, 1, 1, 1, 1, 1); pesos[0] nao e usado.\n");
    printf("Procurando o caminho de 3 ate 2...\n");
    GcaminhoMaisCurto(grafo, pesos, 2, 1);

    GGdestroiGrafo(grafo);
}

int main(void) {
    int opcao;

    do {
        printf("\n=== Teste das TADs ===\n"
               "1 - Pilha\n"
               "2 - Fila\n"
               "3 - Lista\n"
               "4 - Grafo\n"
               "5 - Demonstracao de caminho mais curto\n"
               "0 - Sair\n"
               "Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            return 1;
        }

        switch (opcao) {
            case 1:
                testaPilha();
                break;
            case 2:
                testaFila();
                break;
            case 3:
                testaLista();
                break;
            case 4:
                testaGrafo();
                break;
            case 5:
                executaCaminhoComPesos();
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
