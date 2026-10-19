#include <stdio.h>
#include <stdlib.h>
#include "Grafo.h"

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

/* Caminho minimo em quantidade de arestas, calculado por busca em largura. */
void GcaminhoMaisCurto(Grafo *grafo, int inicio, int chegada) {
    if (grafo == NULL || !GBexisteIdVertice(grafo, inicio) ||
        !GBexisteIdVertice(grafo, chegada)) {
        printf("Grafo ou vertice informado invalido.\n");
        return;
    }

    int quantidadeVertices = GInumeroVertices(grafo);
    int *distancia = (int *)malloc((size_t)quantidadeVertices * sizeof(int));
    int *anterior = (int *)malloc((size_t)quantidadeVertices * sizeof(int));
    int *arestaAnterior = (int *)malloc((size_t)quantidadeVertices * sizeof(int));
    int *fila = (int *)malloc((size_t)quantidadeVertices * sizeof(int));

    if (distancia == NULL || anterior == NULL || arestaAnterior == NULL ||
        fila == NULL) {
        printf("Erro ao alocar memoria para o caminho mais curto.\n");
        free(distancia);
        free(anterior);
        free(arestaAnterior);
        free(fila);
        return;
    }

    for (int v = 0; v < quantidadeVertices; ++v) {
        distancia[v] = -1;
        anterior[v] = -1;
        arestaAnterior[v] = -1;
    }

    int frente = 0;
    int fim = 0;
    distancia[inicio] = 0;
    fila[fim++] = inicio;

    while (frente < fim && distancia[chegada] == -1) {
        int atual = fila[frente++];

        for (int aresta = primeiraArestaPercorrivel(grafo, atual);
             aresta != -1;
             aresta = proximaArestaPercorrivel(grafo, atual, aresta)) {
            int destino = destinoPercorrivel(grafo, atual, aresta);

            if (destino >= 0 && destino < quantidadeVertices &&
                distancia[destino] == -1) {
                distancia[destino] = distancia[atual] + 1;
                anterior[destino] = atual;
                arestaAnterior[destino] = aresta;
                fila[fim++] = destino;
            }
        }
    }

    if (distancia[chegada] == -1) {
        printf("Nao existe caminho de v%d para v%d.\n", inicio, chegada);
    } else {
        int tamanhoCaminho = distancia[chegada] + 1;
        int *caminho = (int *)malloc((size_t)tamanhoCaminho * sizeof(int));

        if (caminho == NULL) {
            printf("Erro ao alocar memoria para reconstruir o caminho.\n");
        } else {
            int atual = chegada;

            for (int i = tamanhoCaminho - 1; i >= 0; --i) {
                caminho[i] = atual;
                atual = anterior[atual];
            }

            printf("Caminho mais curto:\nv%d", caminho[0]);
            for (int i = 1; i < tamanhoCaminho; ++i) {
                int vertice = caminho[i];
                printf(",a%d,v%d", arestaAnterior[vertice], vertice);
            }
            printf("\nDistancia percorrida: %d aresta(s).\n",
                   distancia[chegada]);
            free(caminho);
        }
    }

    free(distancia);
    free(anterior);
    free(arestaAnterior);
    free(fila);
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

/* Menu de texto: carrega um grafo, executa algoritmos e exporta para DOT. */
int main(int argc, char **argv) {
    const char *arquivoEntrada = argc > 1 ? argv[1] : "grafo_tp2.txt";
    Grafo *grafo = GGcarregaGrafo((char *)arquivoEntrada);

    if (grafo == NULL) {
        fprintf(stderr, "Nao foi possivel carregar o arquivo '%s'.\n",
                arquivoEntrada);
        fprintf(stderr,
                "Formato esperado: quantidade de vertices e arestas; depois, uma aresta por linha.\n");
        return 1;
    }

    printf("Grafo carregado: %d vertices e %d arestas.\n",
           GInumeroVertices(grafo), GInumeroArestas(grafo));

    int opcao = -1;
    while (opcao != 0) {
        printf("\n"
               "1 - Busca em largura\n"
               "2 - Busca em profundidade\n"
               "3 - Caminho mais curto\n"
               "4 - Componentes conexos\n"
               "5 - Arvore geradora minima (Kruskal)\n"
               "6 - Salvar grafo em arquivo .dot\n"
               "0 - Sair\n"
               "Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida.\n");
            break;
        }

        if (opcao == 1 || opcao == 2) {
            int inicio;
            printf("Vertice inicial (IDs comecam em 0): ");
            if (scanf("%d", &inicio) != 1) break;

            if (opcao == 1) GbuscaLargura(grafo, inicio);
            else GbuscaProfundidade(grafo, inicio);
        } else if (opcao == 3) {
            int inicio, chegada;
            printf("Vertice inicial e vertice de chegada: ");
            if (scanf("%d%d", &inicio, &chegada) != 2) break;
            GcaminhoMaisCurto(grafo, inicio, chegada);
        } else if (opcao == 4) {
            GcomponentesConexos(grafo);
        } else if (opcao == 5) {
            int quantidadeArestas = GInumeroArestas(grafo);
            float *pesos = quantidadeArestas > 0
                ? (float *)calloc((size_t)quantidadeArestas, sizeof(float))
                : NULL;

            if (quantidadeArestas > 0 && pesos == NULL) {
                printf("Erro ao alocar memoria para os pesos.\n");
                continue;
            }

            printf("Informe o peso de cada aresta pelo ID (a0, a1, ...):\n");
            int entradaValida = 1;
            for (int id = GAprimeiraAresta(grafo); id != -1;
                 id = GAproximaAresta(grafo, id)) {
                printf("Peso da aresta a%d: ", id);
                if (scanf("%f", &pesos[id]) != 1) {
                    entradaValida = 0;
                    break;
                }
            }

            if (entradaValida) Gkruskal(grafo, pesos);
            else printf("Peso invalido.\n");
            free(pesos);
        } else if (opcao == 6) {
            char nomeSaida[256];
            printf("Nome do arquivo .dot para salvar: ");
            if (scanf("%255s", nomeSaida) != 1) break;

            if (GBsalvaGrafoDot(grafo, nomeSaida)) {
                printf("Grafo exportado para '%s'.\n", nomeSaida);
            } else {
                printf("Nao foi possivel salvar o arquivo .dot.\n");
            }
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }
    }

    GGdestroiGrafo(grafo);
    return 0;
}
