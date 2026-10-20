#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "Grafo.h"

#define RESERVA_MANIPULACAO 10

static int leInteiro(const char *mensagem, int *valor) {
    printf("%s", mensagem);
    return scanf("%d", valor) == 1;
}

static void mostraGrafo(Grafo *grafo) {
    printf("\nGrafo %s: %d vertice(s), %d aresta(s)\n",
           GehDirigido(grafo) ? "dirigido" : "nao dirigido",
           GInumeroVertices(grafo), GInumeroArestas(grafo));

    printf("Vertices:\n");
    int vertice = GVprimeiroVertice(grafo);
    if (vertice == -1) {
        printf("  (nenhum)\n");
    }
    while (vertice != -1) {
        printf("  v%d (grau %d)\n", vertice, GIpegaGrau(grafo, vertice));
        vertice = GVproximoVertice(grafo, vertice);
    }

    printf("Arestas:\n");
    int aresta = GAprimeiraAresta(grafo);
    if (aresta == -1) {
        printf("  (nenhuma)\n");
    }
    while (aresta != -1) {
        printf("  a%d: v%d %s v%d\n", aresta, GValfa(grafo, aresta),
               GehDirigido(grafo) ? "->" : "--", GVomega(grafo, aresta));
        aresta = GAproximaAresta(grafo, aresta);
    }
}

static Grafo *copiaComReserva(Grafo *original) {
    int quantidadeVertices = GInumeroVertices(original);
    int quantidadeArestas = GInumeroArestas(original);

    if (quantidadeVertices > INT_MAX - RESERVA_MANIPULACAO ||
        quantidadeArestas > INT_MAX - RESERVA_MANIPULACAO) {
        return NULL;
    }

    Grafo *copia = GGcriaGrafoTipo(
        quantidadeVertices + RESERVA_MANIPULACAO,
        quantidadeArestas + RESERVA_MANIPULACAO,
        GehDirigido(original));
    if (copia == NULL) {
        return NULL;
    }

    for (int vertice = GVprimeiroVertice(original); vertice != -1;
         vertice = GVproximoVertice(original, vertice)) {
        Vertice *novoVertice = GVcriaVertice(copia);
        if (novoVertice == NULL || !GBexisteIdVertice(copia, vertice)) {
            GGdestroiGrafo(copia);
            return NULL;
        }
    }

    for (int aresta = GAprimeiraAresta(original); aresta != -1;
         aresta = GAproximaAresta(original, aresta)) {
        if (GAcriaAresta(copia, GValfa(original, aresta),
                         GVomega(original, aresta)) == NULL) {
            GGdestroiGrafo(copia);
            return NULL;
        }
    }

    return copia;
}

static Grafo *criaGrafoPeloTeclado(void) {
    int quantidadeVertices;
    int quantidadeArestas;
    int dirigido;

    if (!leInteiro("Quantidade de vertices: ", &quantidadeVertices) ||
        !leInteiro("Quantidade de arestas: ", &quantidadeArestas) ||
        !leInteiro("Grafo dirigido? (1=sim, 0=nao): ", &dirigido)) {
        return NULL;
    }

    if (quantidadeVertices < 0 || quantidadeArestas < 0 ||
        quantidadeVertices > INT_MAX - RESERVA_MANIPULACAO ||
        quantidadeArestas > INT_MAX - RESERVA_MANIPULACAO ||
        (dirigido != 0 && dirigido != 1)) {
        printf("Quantidade ou tipo de grafo invalido.\n");
        return NULL;
    }

    Grafo *grafo = GGcriaGrafoTipo(
        quantidadeVertices + RESERVA_MANIPULACAO,
        quantidadeArestas + RESERVA_MANIPULACAO, dirigido);
    if (grafo == NULL) {
        printf("Nao foi possivel criar o grafo.\n");
        return NULL;
    }

    for (int i = 0; i < quantidadeVertices; i++) {
        if (GVcriaVertice(grafo) == NULL) {
            printf("Nao foi possivel criar todos os vertices.\n");
            GGdestroiGrafo(grafo);
            return NULL;
        }
    }

    printf("Informe a origem e o destino de cada aresta.\n");
    for (int i = 0; i < quantidadeArestas; i++) {
        int origem;
        int destino;
        printf("Aresta a%d\n", i);
        if (!leInteiro("  Origem: ", &origem) ||
            !leInteiro("  Destino: ", &destino)) {
            GGdestroiGrafo(grafo);
            return NULL;
        }

        if (GAcriaAresta(grafo, origem, destino) == NULL) {
            printf("Aresta invalida; confira os vertices e duplicatas.\n");
            GGdestroiGrafo(grafo);
            return NULL;
        }
    }

    printf("Grafo criado com espaco para mais %d vertices e arestas.\n",
           RESERVA_MANIPULACAO);
    return grafo;
}

static Grafo *carregaGrafoDeArquivo(void) {
    char nomeArquivo[256];
    printf("Formato: quantidade de vertices e arestas, seguidas "
           "pelas extremidades de cada aresta.\n");
    printf("Arquivo (exemplo: grafo_exemplo.txt): ");
    if (scanf("%255s", nomeArquivo) != 1) {
        printf("Nome de arquivo invalido.\n");
        return NULL;
    }

    Grafo *carregado = GGcarregaGrafo(nomeArquivo);
    if (carregado == NULL) {
        printf("Nao foi possivel carregar '%s'.\n", nomeArquivo);
        return NULL;
    }

    Grafo *grafo = copiaComReserva(carregado);
    GGdestroiGrafo(carregado);
    if (grafo == NULL) {
        printf("Nao foi possivel preparar o grafo para manipulacao.\n");
        return NULL;
    }

    printf("Grafo carregado de '%s' (o formato e interpretado como "
           "nao dirigido).\n", nomeArquivo);
    return grafo;
}

static void menuManipulacao(Grafo *grafo) {
    int opcao;

    for (;;) {
        printf("\n--- Manipular grafo pela TAD ---\n"
               "1 - Exibir vertices e arestas\n"
               "2 - Adicionar vertice\n"
               "3 - Adicionar aresta\n"
               "4 - Remover aresta\n"
               "5 - Remover vertice\n"
               "6 - Consultar aresta\n"
               "7 - Salvar grafo em arquivo .dot\n"
               "0 - Voltar\n");
        if (!leInteiro("Opcao: ", &opcao)) {
            return;
        }

        if (opcao == 0) {
            return;
        } else if (opcao == 1) {
            mostraGrafo(grafo);
        } else if (opcao == 2) {
            if (GVcriaVertice(grafo) == NULL) {
                printf("Nao foi possivel adicionar vertice "
                       "(capacidade atingida).\n");
            } else {
                printf("Vertice adicionado.\n");
                mostraGrafo(grafo);
            }
        } else if (opcao == 3) {
            int origem;
            int destino;
            if (!leInteiro("Vertice de origem: ", &origem) ||
                !leInteiro("Vertice de destino: ", &destino)) {
                return;
            }
            if (GAcriaAresta(grafo, origem, destino) == NULL) {
                printf("Nao foi possivel adicionar a aresta.\n");
            } else {
                printf("Aresta adicionada.\n");
                mostraGrafo(grafo);
            }
        } else if (opcao == 4) {
            int id;
            if (!leInteiro("ID da aresta: ", &id)) {
                return;
            }
            printf(GAremoveAresta(grafo, id)
                       ? "Aresta removida.\n" : "Aresta nao encontrada.\n");
        } else if (opcao == 5) {
            int id;
            if (!leInteiro("ID do vertice: ", &id)) {
                return;
            }
            printf(GVremoveVertice(grafo, id)
                       ? "Vertice e arestas incidentes removidos.\n"
                       : "Vertice nao encontrado.\n");
        } else if (opcao == 6) {
            int origem;
            int destino;
            if (!leInteiro("Primeiro vertice: ", &origem) ||
                !leInteiro("Segundo vertice: ", &destino)) {
                return;
            }
            printf(GBexisteAresta(grafo, origem, destino)
                       ? "Aresta encontrada.\n" : "Aresta nao encontrada.\n");
        } else if (opcao == 7) {
            char nomeArquivo[256];
            printf("Nome do arquivo .dot (exemplo: grafo.dot): ");
            if (scanf("%255s", nomeArquivo) != 1) {
                return;
            }
            if (GBsalvaGrafoDot(grafo, nomeArquivo)) {
                printf("Grafo salvo em '%s'.\n", nomeArquivo);
            } else {
                printf("Nao foi possivel salvar o arquivo.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }
}

int main(void) {
    Grafo *grafo = NULL;
    int opcao;

    for (;;) {
        printf("\n=== TP2: manipulacao de grafos ===\n"
               "1 - Criar grafo pelo teclado\n"
               "2 - Ler grafo de arquivo\n"
               "3 - Manipular grafo pela TAD\n"
               "0 - Sair\n");
        if (!leInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1 || opcao == 2) {
            Grafo *novoGrafo = opcao == 1
                                   ? criaGrafoPeloTeclado()
                                   : carregaGrafoDeArquivo();
            if (novoGrafo != NULL) {
                GGdestroiGrafo(grafo);
                grafo = novoGrafo;
                mostraGrafo(grafo);
            }
        } else if (opcao == 3) {
            if (grafo == NULL) {
                printf("Crie um grafo ou carregue um arquivo primeiro.\n");
            } else {
                menuManipulacao(grafo);
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }

    GGdestroiGrafo(grafo);
    return 0;
}
