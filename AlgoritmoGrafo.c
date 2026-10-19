#include <stdio.h>
#include <stdlib.h>

#include "Grafo.h"

typedef struct {
    int id;
    int origem;
    int destino;
    float peso;
} ArestaKruskal;

/* Encontra o representante de um conjunto */
int encontrar(int pai[], int vertice) {
    if (pai[vertice] != vertice) {
        pai[vertice] = encontrar(pai, pai[vertice]);
    }

    return pai[vertice];
}

/* Une dois conjuntos */
void unir(int pai[], int rank[], int u, int v) {
    int raizU = encontrar(pai, u);
    int raizV = encontrar(pai, v);

    if (raizU == raizV) {
        return;
    }

    if (rank[raizU] < rank[raizV]) {
        pai[raizU] = raizV;
    } else if (rank[raizU] > rank[raizV]) {
        pai[raizV] = raizU;
    } else {
        pai[raizV] = raizU;
        rank[raizU]++;
    }
}

/* Compara as arestas pelo peso */
int compararArestas(const void* a, const void* b) {
    const ArestaKruskal* arestaA = a;
    const ArestaKruskal* arestaB = b;

    if (arestaA->peso < arestaB->peso) {
        return -1;
    }

    if (arestaA->peso > arestaB->peso) {
        return 1;
    }

    return 0;
}

void Gkruskal(Grafo* grafo, float* pesos) {
    if (grafo == NULL || pesos == NULL) {
        return;
    }

    int qtdArestas = GInumeroArestas(grafo);
    int qtdVertices = GInumeroVertices(grafo);

    ArestaKruskal* arestas =
        malloc(qtdArestas * sizeof(ArestaKruskal));

    int* pai = malloc((qtdVertices + 1) * sizeof(int));
    int* rank = calloc(qtdVertices + 1, sizeof(int));

    if ((qtdArestas > 0 && arestas == NULL) ||
        pai == NULL || rank == NULL) {
        free(arestas);
        free(pai);
        free(rank);
        return;
    }

    /* Inicializa os conjuntos dos vértices */
    for (int i = 0; i <= qtdVertices; i++) {
        pai[i] = i;
    }

    /* Copia as arestas do grafo para um vetor ordenável */
    int quantidade = 0;

    for (int id = GAprimeiraAresta(grafo); id != -1; id = GAproximaAresta(grafo, id)) {

        if (quantidade >= qtdArestas) {
            break;
        }

        int origem = GValfa(grafo, id);
        int destino = GVomega(grafo, id);

        arestas[quantidade].id = id;
        arestas[quantidade].origem = origem;
        arestas[quantidade].destino = destino;
        arestas[quantidade].peso = pesos[id];

        quantidade++;
    }

    /* Ordena da menor para a maior aresta */
    qsort(arestas, quantidade, sizeof(ArestaKruskal),
          compararArestas);

    int selecionadas = 0;
    float pesoTotal = 0;

    printf("Arestas da arvore geradora minima:\n");

    for (int i = 0; i < quantidade; i++) {
        int u = arestas[i].origem;
        int v = arestas[i].destino;

        if (u < 1 || u > qtdVertices ||
            v < 1 || v > qtdVertices) {
            continue;
        }

        /* Só adiciona se não formar ciclo */
        if (encontrar(pai, u) != encontrar(pai, v)) {
            printf("%d -- %d [label=\"%.1f\"];\n",
                   u, v, arestas[i].peso);

            unir(pai, rank, u, v);

            pesoTotal += arestas[i].peso;
            selecionadas++;

            if (selecionadas == GInumeroVertices(grafo) - 1) {
                break;
            }
        }
    }

    if (selecionadas != GInumeroVertices(grafo) - 1) {
        printf("O grafo nao e conexo: nao existe arvore "
               "geradora para todos os vertices.\n");
    } else {
        printf("Peso total: %.2f\n", pesoTotal);
    }

    free(arestas);
    free(pai);
    free(rank);
}

void GbuscaLargura(Grafo* grafo, int inicio) {
    if (grafo == NULL) {
        return;
    }

    int maxVertices = GInumeroVertices(grafo);

    if (!GBexisteIdVertice(grafo, inicio)) {
        printf("Vertice inicial inexistente.\n");
        return;
    }

    int* visitado = calloc(maxVertices + 1, sizeof(int));
    int* fila = malloc((maxVertices + 1) * sizeof(int));

    if (visitado == NULL || fila == NULL) {
        free(visitado);
        free(fila);
        return;
    }

    int frente = 0;
    int tras = 0;

    fila[tras] = inicio;
    tras++;

    visitado[inicio] = 1;

    printf("v%d", inicio);

    while (frente < tras) {
        int atual = fila[frente];
        frente++;

        int aresta;

        if (GehDirigido(grafo)) {
            aresta = GAprimaSaida(grafo, atual);
        } else {
            aresta = GAprimaAresta(grafo, atual);
        }

        while (aresta != -1) {
            int vizinho = GVvizinho(grafo, atual, aresta);

            if (!visitado[vizinho]) {
                visitado[vizinho] = 1;

                printf(", a%d, v%d", aresta, vizinho);

                fila[tras] = vizinho;
                tras++;
            }

            if (GehDirigido(grafo)) {
                aresta = GAproxSaida(grafo, atual, aresta);
            } else {
                aresta = GAproxAresta(grafo, atual, aresta);
            }
        }
    }

    printf("\n");

    free(visitado);
    free(fila);
}


void main() {

    Grafo* grafo = GGcarregaGrafo("grafo1.dot");
    float pesos[16] = {
        0.0,  // índice 0: não utilizado
        4.5,  // aresta 1
        2.0,  // aresta 2
        7.3,  // aresta 3
        1.5,  // aresta 4
        3.2,  // aresta 5
        6.0,  // aresta 6
        2.8,  // aresta 7
        5.1,  // aresta 8
        1.0,  // aresta 9
        8.4,  // aresta 10
        3.7,  // aresta 11
        4.0,  // aresta 12
        9.2,  // aresta 13
        2.3,  // aresta 14
        6.5   // aresta 15
    };
    GbuscaLargura(grafo, 1);

    GBsalvaGrafo(grafo, "teste.dot");
    

}