#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "AlgoritmoGrafo.h"

/* Retorna a primeira aresta que pode ser percorrida a partir de v. */
static int primeiraArestaPercorrivel(Grafo *grafo, int v) {
    return GehDirigido(grafo) ? GAprimaSaida(grafo, v)
                              : GAprimaAresta(grafo, v);
}

/* Retorna a proxima aresta de saida/incidencia, conforme o tipo do grafo. */
static int proximaArestaPercorrivel(Grafo *grafo, int v, int aresta) {
    return GehDirigido(grafo) ? GAproxSaida(grafo, v, aresta)
                              : GAproxAresta(grafo, v, aresta);
}

/* Determina o vertice alcancado ao percorrer uma aresta a partir de v. */
static int destinoPercorrivel(Grafo *grafo, int v, int aresta) {
    return GehDirigido(grafo) ? GVomega(grafo, aresta)
                              : GVvizinho(grafo, v, aresta);
}

/* Busca em largura: registra cada aresta usada para descobrir um vertice. */
void GbuscaLargura(Grafo *grafo, int inicio) {
    if (grafo == NULL || !GBexisteIdVertice(grafo, inicio)) {
        printf("Grafo ou vertice inicial invalido.\n");
        return;
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    int *visitado = (int *)calloc((size_t)quantidadeVertices, sizeof(int));
    int *fila = (int *)malloc((size_t)quantidadeVertices * sizeof(int));

    if (visitado == NULL || fila == NULL) {
        printf("Erro ao alocar memoria para a busca em largura.\n");
        free(visitado);
        free(fila);
        return;
    }

    int frente = 0;
    int fim = 0;

    visitado[inicio] = 1;
    fila[fim++] = inicio;

    printf("Historico da busca em largura:\nv%d", inicio);

    while (frente < fim) {
        int atual = fila[frente++];

        for (int aresta = primeiraArestaPercorrivel(grafo, atual);
             aresta != -1;
             aresta = proximaArestaPercorrivel(grafo, atual, aresta)) {
            int destino = destinoPercorrivel(grafo, atual, aresta);

            if (destino >= 0 && destino < quantidadeVertices &&
                !visitado[destino]) {
                visitado[destino] = 1;
                fila[fim++] = destino;
                printf(",a%d,v%d", aresta, destino);
            }
        }
    }

    printf("\n");
    free(visitado);
    free(fila);
}

/* DFS recursiva; imprime a aresta e o vertice no momento da descoberta. */
static void buscaProfundidadeRecursiva(Grafo *grafo, int atual,
                                      int *visitado) {
    visitado[atual] = 1;

    for (int aresta = primeiraArestaPercorrivel(grafo, atual);
         aresta != -1;
         aresta = proximaArestaPercorrivel(grafo, atual, aresta)) {
        int destino = destinoPercorrivel(grafo, atual, aresta);

        if (destino >= 0 && destino < GInumeroVertices(grafo) &&
            !visitado[destino]) {
            printf(",a%d,v%d", aresta, destino);
            buscaProfundidadeRecursiva(grafo, destino, visitado);
        }
    }
}

/* Busca em profundidade a partir do vertice informado. */
void GbuscaProfundidade(Grafo *grafo, int inicio) {
    if (grafo == NULL || !GBexisteIdVertice(grafo, inicio)) {
        printf("Grafo ou vertice inicial invalido.\n");
        return;
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    int *visitado = (int *)calloc((size_t)quantidadeVertices, sizeof(int));

    if (visitado == NULL) {
        printf("Erro ao alocar memoria para a busca em profundidade.\n");
        return;
    }

    printf("Historico da busca em profundidade:\nv%d", inicio);
    buscaProfundidadeRecursiva(grafo, inicio, visitado);
    printf("\n");

    free(visitado);
}

/* Menor caminho por distancia total, usando Dijkstra para pesos nao negativos. */
void GcaminhoMaisCurto(Grafo *grafo, const float *pesos,
                       int inicio, int chegada) {
    if (grafo == NULL || pesos == NULL ||
        !GBexisteIdVertice(grafo, inicio) ||
        !GBexisteIdVertice(grafo, chegada)) {
        printf("Grafo, pesos ou vertice informado invalido.\n");
        return;
    }

    for (int aresta = GAprimeiraAresta(grafo); aresta != -1;
         aresta = GAproximaAresta(grafo, aresta)) {
        float peso = pesos[aresta + 1];
        if (!(peso >= 0.0f) || peso > FLT_MAX) {
            printf("Peso invalido na aresta a%d. Use pesos nao negativos.\n",
                   aresta);
            return;
        }
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    double *distancia = malloc((size_t)quantidadeVertices * sizeof(double));
    int *anterior = malloc((size_t)quantidadeVertices * sizeof(int));
    int *visitado = calloc((size_t)quantidadeVertices, sizeof(int));
    int *caminho = malloc((size_t)quantidadeVertices * sizeof(int));

    if (distancia == NULL || anterior == NULL ||
        visitado == NULL || caminho == NULL) {
        printf("Erro ao alocar memoria para o caminho mais curto.\n");
        free(distancia);
        free(anterior);
        free(visitado);
        free(caminho);
        return;
    }

    for (int v = 0; v < quantidadeVertices; ++v) {
        distancia[v] = DBL_MAX;
        anterior[v] = -1;
    }

    distancia[inicio] = 0;
    for (int passo = 0; passo < quantidadeVertices; ++passo) {
        int atual = -1;
        for (int v = 0; v < quantidadeVertices; ++v) {
            if (!visitado[v] &&
                (atual == -1 || distancia[v] < distancia[atual])) {
                atual = v;
            }
        }

        if (atual == -1 || distancia[atual] == DBL_MAX) {
            break;
        }
        if (atual == chegada) {
            break;
        }
        visitado[atual] = 1;

        for (int aresta = primeiraArestaPercorrivel(grafo, atual);
             aresta != -1;
             aresta = proximaArestaPercorrivel(grafo, atual, aresta)) {
            int destino = destinoPercorrivel(grafo, atual, aresta);

            if (destino < 0 || destino >= quantidadeVertices ||
                visitado[destino]) {
                continue;
            }

            double novaDistancia = distancia[atual] + pesos[aresta + 1];
            if (novaDistancia < distancia[destino]) {
                distancia[destino] = novaDistancia;
                anterior[destino] = atual;
            }
        }
    }

    if (distancia[chegada] == DBL_MAX) {
        printf("Caminho mais curto: %d\n", inicio);
    } else {
        int tamanhoCaminho = 0;
        for (int atual = chegada; atual != -1; atual = anterior[atual]) {
            caminho[tamanhoCaminho++] = atual;
        }

        printf("Caminho mais curto: %d", caminho[tamanhoCaminho - 1]);
        for (int i = tamanhoCaminho - 2; i >= 0; --i) {
            printf(" -> %d", caminho[i]);
        }
        printf("\nDistancia total: %.2f\n", distancia[chegada]);
    }

    free(distancia);
    free(anterior);
    free(visitado);
    free(caminho);
}

/*
 * Encontra componentes conexos. Em grafo dirigido, considera as arestas
 * nos dois sentidos, isto e, calcula componentes fracamente conexos.
 */
void GcomponentesConexos(Grafo *grafo) {
    if (grafo == NULL) {
        printf("Grafo invalido.\n");
        return;
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    int *visitado = (int *)calloc((size_t)quantidadeVertices, sizeof(int));
    int *fila = (int *)malloc((size_t)quantidadeVertices * sizeof(int));

    if (visitado == NULL || fila == NULL) {
        printf("Erro ao alocar memoria para os componentes conexos.\n");
        free(visitado);
        free(fila);
        return;
    }

    printf("Componentes conexos:\n");

    for (int inicio = GVprimeiroVertice(grafo); inicio != -1;
         inicio = GVproximoVertice(grafo, inicio)) {
        if (visitado[inicio]) {
            continue;
        }

        int frente = 0;
        int fim = 0;
        int primeiro = 1;
        fila[fim++] = inicio;
        visitado[inicio] = 1;
        printf("{");

        while (frente < fim) {
            int atual = fila[frente++];
            printf("%s%d", primeiro ? "" : ",", atual);
            primeiro = 0;

            /* A lista de incidencias permite tratar o grafo como nao dirigido. */
            for (int aresta = GAprimaAresta(grafo, atual); aresta != -1;
                 aresta = GAproxAresta(grafo, atual, aresta)) {
                int vizinho = GVvizinho(grafo, atual, aresta);

                if (vizinho >= 0 && vizinho < quantidadeVertices &&
                    !visitado[vizinho]) {
                    visitado[vizinho] = 1;
                    fila[fim++] = vizinho;
                }
            }
        }

        printf("}\n");
    }

    free(visitado);
    free(fila);
}

typedef struct {
    int id;
    int origem;
    int destino;
    float peso;
} ArestaPonderada;

/* Compara arestas por peso para ordenar a lista usada por Kruskal. */
static int compararArestas(const void *a, const void *b) {
    const ArestaPonderada *arestaA = (const ArestaPonderada *)a;
    const ArestaPonderada *arestaB = (const ArestaPonderada *)b;

    if (arestaA->peso < arestaB->peso) return -1;
    if (arestaA->peso > arestaB->peso) return 1;
    return 0;
}

/* Procura a raiz do conjunto e comprime o caminho para acelerar consultas. */
static int encontrarRaiz(int *pai, int vertice) {
    if (pai[vertice] != vertice) {
        pai[vertice] = encontrarRaiz(pai, pai[vertice]);
    }
    return pai[vertice];
}

/* AGM de Kruskal. Os pesos sao informados pelo usuario no menu. */
void Gkruskal(Grafo *grafo, const float *pesos) {
    if (grafo == NULL || (GInumeroArestas(grafo) > 0 && pesos == NULL)) {
        printf("Grafo ou lista de pesos invalida.\n");
        return;
    }

    if (GehDirigido(grafo)) {
        printf("Kruskal requer um grafo nao dirigido.\n");
        return;
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    int quantidadeArestas = GInumeroArestas(grafo);
    ArestaPonderada *arestas = quantidadeArestas > 0
        ? (ArestaPonderada *)malloc((size_t)quantidadeArestas * sizeof(*arestas))
        : NULL;
    int *pai = (int *)malloc((size_t)quantidadeVertices * sizeof(int));
    int *altura = (int *)calloc((size_t)quantidadeVertices, sizeof(int));

    if ((quantidadeArestas > 0 && arestas == NULL) || pai == NULL ||
        altura == NULL) {
        printf("Erro ao alocar memoria para Kruskal.\n");
        free(arestas);
        free(pai);
        free(altura);
        return;
    }

    for (int v = 0; v < quantidadeVertices; ++v) {
        pai[v] = v;
    }

    int quantidadeLida = 0;
    for (int id = GAprimeiraAresta(grafo); id != -1;
         id = GAproximaAresta(grafo, id)) {
        arestas[quantidadeLida].id = id;
        arestas[quantidadeLida].origem = GValfa(grafo, id);
        arestas[quantidadeLida].destino = GVomega(grafo, id);
        arestas[quantidadeLida].peso = pesos[id];
        ++quantidadeLida;
    }

    if (quantidadeLida > 1) {
        qsort(arestas, (size_t)quantidadeLida, sizeof(*arestas), compararArestas);
    }

    printf("Arestas da arvore geradora minima:\n");
    int selecionadas = 0;
    float pesoTotal = 0.0f;

    for (int i = 0; i < quantidadeLida &&
         selecionadas < quantidadeVertices - 1; ++i) {
        int raizOrigem = encontrarRaiz(pai, arestas[i].origem);
        int raizDestino = encontrarRaiz(pai, arestas[i].destino);

        /* Se as raizes forem diferentes, a aresta nao cria um ciclo. */
        if (raizOrigem != raizDestino) {
            if (altura[raizOrigem] < altura[raizDestino]) {
                pai[raizOrigem] = raizDestino;
            } else if (altura[raizOrigem] > altura[raizDestino]) {
                pai[raizDestino] = raizOrigem;
            } else {
                pai[raizDestino] = raizOrigem;
                ++altura[raizOrigem];
            }

            printf("v%d -- v%d (a%d, peso %.2f)\n",
                   arestas[i].origem, arestas[i].destino,
                   arestas[i].id, arestas[i].peso);
            pesoTotal += arestas[i].peso;
            ++selecionadas;
        }
    }

    if (selecionadas != quantidadeVertices - 1) {
        printf("O grafo e desconexo; nao existe AGM para todos os vertices.\n");
    } else {
        printf("Peso total: %.2f\n", pesoTotal);
    }

    free(arestas);
    free(pai);
    free(altura);
}
