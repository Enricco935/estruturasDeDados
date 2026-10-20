// Implementação de TADs simples e grafo

// Alunos:
// Enricco Augusto Rodrigues Faria  | RA: 0117036
// João Vitor Da Silva Silverio     | RA: 0117181
// 19/10/2026

// Professor:
// Walace de Almeida Rodrigues

#include <stdlib.h>
#include <stdio.h>
#include "Grafo.h"

/* Estruturas internas da TAD */
typedef struct incidencia {
    Aresta* aresta;
    struct incidencia* proxima;
} Incidencia;
struct aresta {
    int id;
    int alfa;
    int omega;
};
struct vertice {
    int id;

    Incidencia* incidencias;
    Incidencia* entradas;
    Incidencia* saidas;
};
struct grafo {
    int dirigido;

    int qtdVertices;
    int maxVertices;

    int qtdArestas;
    int maxArestas;

    int proximoIdVertice;
    int proximoIdAresta;

    Vertice** vertices;
    Aresta** arestas;
};

/* Função auxiliar: libera uma lista de incidências. */
static void liberaListaIncidencias(Incidencia* lista) {
    while (lista != NULL) {
        Incidencia* proxima = lista->proxima;

        free(lista);

        lista = proxima;
    }
}
/* Cria um grafo não dirigido, preservando a assinatura original. */
Grafo* GGcriaGrafo(int v, int a) {
    return GGcriaGrafoTipo(v, a, 0);
}
/*
 * Cria um grafo dirigido ou não dirigido.
 *
 * v: capacidade máxima de vértices.
 * a: capacidade máxima de arestas.
 * dirigido: 1 para dirigido; 0 para não dirigido.
 *
 * Retorna NULL se os parâmetros forem inválidos
 * ou se não houver memória suficiente.
 */

Grafo* GGcriaGrafoTipo(int v, int a, int dirigido) {
    if (v < 0 || a < 0) {
        return NULL;
    }

    if (dirigido != 0 && dirigido != 1) {
        return NULL;
    }

    Grafo* grafo = malloc(sizeof(Grafo));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->dirigido = dirigido;

    grafo->proximoIdVertice = 0;
    grafo->proximoIdAresta = 0;

    grafo->qtdVertices = 0;
    grafo->maxVertices = v;

    grafo->qtdArestas = 0;
    grafo->maxArestas = a;

    grafo->vertices = NULL;
    grafo->arestas = NULL;

    if (v > 0) {
        grafo->vertices = malloc(v * sizeof(Vertice*));

        if (grafo->vertices == NULL) {
            free(grafo);
            return NULL;
        }
    }

    if (a > 0) {
        grafo->arestas = malloc(a * sizeof(Aresta*));

        if (grafo->arestas == NULL) {
            free(grafo->vertices);
            free(grafo);
            return NULL;
        }
    }

    return grafo;
}
/* Informa se o grafo é dirigido. */
int GehDirigido(Grafo* grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo->dirigido;
}
/* Destrói o grafo e libera toda a memória alocada. */
void GGdestroiGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }

    /* Libera as listas de incidências de cada vértice. */

    for (int i = 0; i < grafo->qtdVertices; i++) {
        Vertice* vertice = grafo->vertices[i];

        if (vertice != NULL) {
            liberaListaIncidencias(vertice->incidencias);
            liberaListaIncidencias(vertice->entradas);
            liberaListaIncidencias(vertice->saidas);

            free(vertice);
        }
    }

    /* Libera cada aresta uma única vez. */

    for (int i = 0; i < grafo->qtdArestas; i++) {
        free(grafo->arestas[i]);
    }

    /* Libera os vetores e a estrutura do grafo. */

    free(grafo->vertices);
    free(grafo->arestas);
    free(grafo);
}
/* Cria um vértice e o adiciona ao grafo. */
Vertice* GVcriaVertice(Grafo* grafo) {
    if (grafo == NULL) {
        return NULL;
    }

    /* Verifica se existe espaço para mais um vértice. */
    if (grafo->qtdVertices >= grafo->maxVertices) {
        return NULL;
    }

    Vertice* vertice = malloc(sizeof(Vertice));

    if (vertice == NULL) {
        return NULL;
    }

    /* Inicializa o vértice. */
    vertice->id = grafo->proximoIdVertice;

    vertice->incidencias = NULL;
    vertice->entradas = NULL;
    vertice->saidas = NULL;

    /* Adiciona o vértice ao vetor do grafo. */
    grafo->vertices[grafo->qtdVertices] = vertice;

    grafo->qtdVertices++;
    grafo->proximoIdVertice++;

    return vertice;
}
/* Verifica se existe um vértice com o ID informado. */
int GBexisteIdVertice(Grafo* grafo, int id) {
    if (grafo == NULL) {
        return 0;
    }

    for (int i = 0; i < grafo->qtdVertices; i++) {
        if (grafo->vertices[i]->id == id) {
            return 1;
        }
    }

    return 0;
}
/* Retorna o ID do primeiro vértice do grafo. */
int GVprimeiroVertice(Grafo* grafo) {
    if (grafo == NULL || grafo->qtdVertices == 0) {
        return -1;
    }

    return grafo->vertices[0]->id;
}
int GVproximoVertice(Grafo* grafo, int v1) {
    if (grafo == NULL) {
        return -1;
    }

    int proximo = -1;

    for (int i = 0; i < grafo->qtdVertices; i++) {
        int id = grafo->vertices[i]->id;

        if (id > v1) {
            if (proximo == -1 || id < proximo) {
                proximo = id;
            }
        }
    }

    return proximo;
}

/* =========================================================
   FUNÇÕES AUXILIARES
   ========================================================= */

/* Localiza um vértice pelo ID. */
static Vertice* buscaVerticePorId(Grafo* grafo, int id) {
    if (grafo == NULL) {
        return NULL;
    }

    for (int i = 0; i < grafo->qtdVertices; i++) {
        if (grafo->vertices[i]->id == id) {
            return grafo->vertices[i];
        }
    }

    return NULL;
}
/* Localiza uma aresta pelo ID. */
static Aresta* buscaArestaPorId(Grafo* grafo, int id) {
    if (grafo == NULL) {
        return NULL;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->arestas[i]->id == id) {
            return grafo->arestas[i];
        }
    }

    return NULL;
}
/* Cria um nó de incidência. */
static Incidencia* criaIncidencia(Aresta* aresta) {
    Incidencia* incidencia = malloc(sizeof(Incidencia));

    if (incidencia == NULL) {
        return NULL;
    }

    incidencia->aresta = aresta;
    incidencia->proxima = NULL;

    return incidencia;
}
/* Insere um nó no início de uma lista de incidências. */
static void insereIncidencia(
    Incidencia** lista,
    Incidencia* incidencia
) {
    incidencia->proxima = *lista;
    *lista = incidencia;
}

/* =========================================================
   GBexisteIdAresta
   ========================================================= */

/* Verifica se existe uma aresta com o ID informado. */
int GBexisteIdAresta(Grafo* grafo, int id) {
    if (grafo == NULL) {
        return 0;
    }

    return buscaArestaPorId(grafo, id) != NULL;
}


/* =========================================================
   GAcriaAresta
   ========================================================= */

/*
 * Cria uma aresta entre dois vértices existentes.
 *
 * Retorna:
 * - Ponteiro para a aresta criada em caso de sucesso.
 * - NULL em caso de erro.
 */
Aresta* GAcriaAresta(Grafo* grafo, int v1, int v2) {
    if (grafo == NULL) {
        return NULL;
    }

    /* Verifica se os dois vértices existem. */
    Vertice* origem = buscaVerticePorId(grafo, v1);
    Vertice* destino = buscaVerticePorId(grafo, v2);

    if (origem == NULL || destino == NULL) {
        return NULL;
    }

    /* Verifica se ainda existe espaço para uma aresta. */
    if (grafo->qtdArestas >= grafo->maxArestas) {
        return NULL;
    }

    /*
     * Não permite arestas paralelas.
     * Em grafos não dirigidos, a ordem dos vértices não importa.
     */
    if (GBexisteAresta(grafo, v1, v2)) {
        return NULL;
    }

    /* Aloca a estrutura da aresta. */
    Aresta* aresta = malloc(sizeof(Aresta));

    if (aresta == NULL) {
        return NULL;
    }

    aresta->id = grafo->proximoIdAresta;
    aresta->alfa = v1;
    aresta->omega = v2;

    /*
     * Prepara os nós de incidência antes de alterar o grafo.
     *
     * Para uma aresta comum:
     * - Duas incidências gerais.
     * - Uma entrada e uma saída em grafo dirigido.
     * - Duas entradas e duas saídas em grafo não dirigido.
     *
     * Para um laço:
     * - Duas incidências gerais no mesmo vértice.
     * - Uma entrada e uma saída em grafo dirigido.
     * - Duas entradas e duas saídas em grafo não dirigido.
     */

    Incidencia* nos[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    int qtdNos = 0;

    /* Incidências gerais nas duas extremidades. */
    nos[qtdNos] = criaIncidencia(aresta);

    if (nos[qtdNos] == NULL) {
        free(aresta);
        return NULL;
    }

    qtdNos++;

    nos[qtdNos] = criaIncidencia(aresta);

    if (nos[qtdNos] == NULL) {
        for (int i = 0; i < qtdNos; i++) {
            free(nos[i]);
        }

        free(aresta);
        return NULL;
    }

    qtdNos++;

    if (grafo->dirigido) {
        /* Uma saída da origem e uma entrada no destino. */
        nos[qtdNos] = criaIncidencia(aresta);

        if (nos[qtdNos] == NULL) {
            for (int i = 0; i < qtdNos; i++) {
                free(nos[i]);
            }

            free(aresta);
            return NULL;
        }

        qtdNos++;

        nos[qtdNos] = criaIncidencia(aresta);

        if (nos[qtdNos] == NULL) {
            for (int i = 0; i < qtdNos; i++) {
                free(nos[i]);
            }

            free(aresta);
            return NULL;
        }

        qtdNos++;
    } else {
        /*
         * Em grafos não dirigidos, entradas e saídas são
         * tratadas como listas de incidências para permitir
         * percorrer as arestas por qualquer extremidade.
         */

        for (int i = 0; i < 4; i++) {
            nos[qtdNos] = criaIncidencia(aresta);

            if (nos[qtdNos] == NULL) {
                for (int j = 0; j < qtdNos; j++) {
                    free(nos[j]);
                }

                free(aresta);
                return NULL;
            }

            qtdNos++;
        }
    }

    /*
     * Todas as alocações foram bem-sucedidas.
     * Agora podemos registrar a aresta no grafo.
     */

    grafo->arestas[grafo->qtdArestas] = aresta;
    grafo->qtdArestas++;
    grafo->proximoIdAresta++;

    /* Registra as duas extremidades nas listas gerais. */
    insereIncidencia(&origem->incidencias, nos[0]);
    insereIncidencia(&destino->incidencias, nos[1]);

    if (grafo->dirigido) {
        /* nos[2]: saída da origem. */
        insereIncidencia(&origem->saidas, nos[2]);

        /* nos[3]: entrada no destino. */
        insereIncidencia(&destino->entradas, nos[3]);
    } else {
        /* Entradas: uma incidência em cada extremidade. */
        insereIncidencia(&origem->entradas, nos[2]);
        insereIncidencia(&destino->entradas, nos[3]);

        /* Saídas: uma incidência em cada extremidade. */
        insereIncidencia(&origem->saidas, nos[4]);
        insereIncidencia(&destino->saidas, nos[5]);
    }

    return aresta;
}


/* =========================================================
   GBexisteArestaDir
   ========================================================= */

/* Verifica se existe uma aresta orientada de v1 para v2. */
int GBexisteArestaDir(Grafo* grafo, int v1, int v2) {
    if (grafo == NULL) {
        return 0;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (aresta->alfa == v1 && aresta->omega == v2) {
            return 1;
        }
    }

    return 0;
}


/* =========================================================
   GBexisteAresta
   ========================================================= */

/*
 * Grafo dirigido:
 * verifica se existe uma aresta de v1 para v2.
 *
 * Grafo não dirigido:
 * verifica se existe uma aresta entre v1 e v2,
 * independentemente da ordem dos vértices.
 */
int GBexisteAresta(Grafo* grafo, int v1, int v2) {
    if (grafo == NULL) {
        return 0;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (grafo->dirigido) {
            if (aresta->alfa == v1 && aresta->omega == v2) {
                return 1;
            }
        } else {
            if ((aresta->alfa == v1 && aresta->omega == v2) ||
                (aresta->alfa == v2 && aresta->omega == v1)) {
                return 1;
            }
        }
    }

    return 0;
}


/* =========================================================
   GApegaArestaDir
   ========================================================= */

/* Retorna a aresta orientada de v1 para v2. */
Aresta* GApegaArestaDir(Grafo* grafo, int v1, int v2) {
    if (grafo == NULL) {
        return NULL;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (aresta->alfa == v1 && aresta->omega == v2) {
            return aresta;
        }
    }

    return NULL;
}


/* =========================================================
   GApegaAresta
   ========================================================= */

/* Retorna a aresta entre v1 e v2, respeitando o tipo de grafo. */
Aresta* GApegaAresta(Grafo* grafo, int v1, int v2) {
    if (grafo == NULL) {
        return NULL;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (grafo->dirigido) {
            if (aresta->alfa == v1 && aresta->omega == v2) {
                return aresta;
            }
        } else {
            if ((aresta->alfa == v1 && aresta->omega == v2) ||
                (aresta->alfa == v2 && aresta->omega == v1)) {
                return aresta;
            }
        }
    }

    return NULL;
}


/* =========================================================
   GAprimeiraAresta
   ========================================================= */

/* Retorna o menor ID de aresta existente. */
int GAprimeiraAresta(Grafo* grafo) {
    if (grafo == NULL || grafo->qtdArestas == 0) {
        return -1;
    }

    int menorId = grafo->arestas[0]->id;

    for (int i = 1; i < grafo->qtdArestas; i++) {
        if (grafo->arestas[i]->id < menorId) {
            menorId = grafo->arestas[i]->id;
        }
    }

    return menorId;
}


/* =========================================================
   GAproximaAresta
   ========================================================= */

/*
 * Retorna o menor ID de aresta estritamente maior que a1.
 * Retorna -1 se não houver outra aresta.
 */
int GAproximaAresta(Grafo* grafo, int a1) {
    if (grafo == NULL) {
        return -1;
    }

    int proxima = -1;

    for (int i = 0; i < grafo->qtdArestas; i++) {
        int id = grafo->arestas[i]->id;

        if (id > a1 && (proxima == -1 || id < proxima)) {
            proxima = id;
        }
    }

    return proxima;
}

int GInumeroVertices(Grafo* grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo->qtdVertices;
}

int GInumeroVerticesMax(Grafo* grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo->maxVertices;
}

int GInumeroArestas(Grafo* grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo->qtdArestas;
}

int GInumeroArestasMax(Grafo* grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo->maxArestas;
}

int GIpegaGrau(Grafo* grafo, int v) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    int grau = 0;
    Incidencia* atual = vertice->incidencias;

    while (atual != NULL) {
        grau++;
        atual = atual->proxima;
    }

    return grau;
}

static int primeiraArestaIncidencia(Incidencia* lista) {
    if (lista == NULL) {
        return -1;
    }

    int menorId = lista->aresta->id;
    Incidencia* atual = lista->proxima;

    while (atual != NULL) {
        if (atual->aresta->id < menorId) {
            menorId = atual->aresta->id;
        }

        atual = atual->proxima;
    }

    return menorId;
}

static int proximaArestaIncidencia(Incidencia* lista, int a1) {
    int proximoId = -1;
    Incidencia* atual = lista;

    while (atual != NULL) {
        int id = atual->aresta->id;

        if (id > a1 && (proximoId == -1 || id < proximoId)) {
            proximoId = id;
        }

        atual = atual->proxima;
    }

    return proximoId;
}

int GAprimaAresta(Grafo* grafo, int v) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return primeiraArestaIncidencia(vertice->incidencias);
}

int GAproxAresta(Grafo* grafo, int v, int a1) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return proximaArestaIncidencia(vertice->incidencias, a1);
}

int GAprimaEntrada(Grafo* grafo, int v) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return primeiraArestaIncidencia(vertice->entradas);
}

int GAproxEntrada(Grafo* grafo, int v, int a1) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return proximaArestaIncidencia(vertice->entradas, a1);
}

int GAprimaSaida(Grafo* grafo, int v) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return primeiraArestaIncidencia(vertice->saidas);
}

int GAproxSaida(Grafo* grafo, int v, int a1) {
    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return -1;
    }

    return proximaArestaIncidencia(vertice->saidas, a1);
}

int GBarestaLaco(Grafo* grafo, int a) {
    Aresta* aresta = buscaArestaPorId(grafo, a);

    if (aresta == NULL) {
        return -1;
    }

    if (aresta->alfa == aresta->omega) {
        return 1;
    }

    return 0;
}

int GValfa(Grafo* grafo, int a) {
    Aresta* aresta = buscaArestaPorId(grafo, a);

    if (aresta == NULL) {
        return -1;
    }

    return aresta->alfa;
}

int GVomega(Grafo* grafo, int a) {
    Aresta* aresta = buscaArestaPorId(grafo, a);

    if (aresta == NULL) {
        return -1;
    }

    return aresta->omega;
}

int GVvizinho(Grafo* grafo, int v, int a) {
    Aresta* aresta = buscaArestaPorId(grafo, a);

    if (aresta == NULL) {
        return -1;
    }

    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    if (aresta->alfa == v) {
        return aresta->omega;
    }

    if (aresta->omega == v) {
        return aresta->alfa;
    }

    return -1;
}

static void removeIncidenciasAresta(Incidencia** lista, Aresta* aresta) {
    Incidencia* atual = *lista;
    Incidencia* anterior = NULL;

    while (atual != NULL) {
        if (atual->aresta == aresta) {
            Incidencia* remover = atual;

            if (anterior == NULL) {
                *lista = atual->proxima;
            } else {
                anterior->proxima = atual->proxima;
            }

            atual = atual->proxima;
            free(remover);
        } else {
            anterior = atual;
            atual = atual->proxima;
        }
    }
}

static void removeArestaInterna(Grafo* grafo, Aresta* aresta) {
    /* Remove as incidências de todos os vértices. */
    for (int i = 0; i < grafo->qtdVertices; i++) {
        Vertice* vertice = grafo->vertices[i];

        removeIncidenciasAresta(&vertice->incidencias, aresta);
        removeIncidenciasAresta(&vertice->entradas, aresta);
        removeIncidenciasAresta(&vertice->saidas, aresta);
    }

    /* Procura a aresta no vetor de arestas. */
    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->arestas[i] == aresta) {
            /* Desloca as arestas seguintes para a esquerda. */
            for (int j = i; j < grafo->qtdArestas - 1; j++) {
                grafo->arestas[j] = grafo->arestas[j + 1];
            }

            grafo->qtdArestas--;
            break;
        }
    }

    /* Libera a memória da aresta. */
    free(aresta);
}

int GAremoveAresta(Grafo* grafo, int a) {
    if (grafo == NULL) {
        return 0;
    }

    Aresta* aresta = buscaArestaPorId(grafo, a);

    if (aresta == NULL) {
        return 0;
    }

    removeArestaInterna(grafo, aresta);

    return 1;
}

int GVremoveVertice(Grafo* grafo, int v) {
    if (grafo == NULL) {
        return 0;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice == NULL) {
        return 0;
    }

    /*
     * Remove todas as arestas que possuem
     * o vértice como alfa ou omega.
     */
    int i = 0;

    while (i < grafo->qtdArestas) {
        Aresta* aresta = grafo->arestas[i];

        if (aresta->alfa == v || aresta->omega == v) {
            removeArestaInterna(grafo, aresta);
        } else {
            i++;
        }
    }

    /*
     * Procura o vértice no vetor de vértices.
     */
    for (int i = 0; i < grafo->qtdVertices; i++) {
        if (grafo->vertices[i] == vertice) {

            /*
             * Desloca os vértices seguintes
             * para a esquerda.
             */
            for (int j = i; j < grafo->qtdVertices - 1; j++) {
                grafo->vertices[j] = grafo->vertices[j + 1];
            }

            grafo->qtdVertices--;
            break;
        }
    }

    /* Libera a memória do vértice. */
    free(vertice);

    return 1;
}

Grafo* GGcarregaGrafo(char* f) {
    if (f == NULL) {
        return NULL;
    }

    FILE* arquivo = fopen(f, "r");

    if (arquivo == NULL) {
        return NULL;
    }

    int qtdVertices;
    int qtdArestas;

    if (fscanf(arquivo, "%d %d", &qtdVertices, &qtdArestas) != 2) {
        fclose(arquivo);
        return NULL;
    }

    if (qtdVertices < 0 || qtdArestas < 0) {
        fclose(arquivo);
        return NULL;
    }

    Grafo* grafo = GGcriaGrafo(qtdVertices, qtdArestas);

    if (grafo == NULL) {
        fclose(arquivo);
        return NULL;
    }

    /* Cria os vértices. */
    for (int i = 0; i < qtdVertices; i++) {
        if (GVcriaVertice(grafo) == NULL) {
            fclose(arquivo);
            GGdestroiGrafo(grafo);
            return NULL;
        }
    }

    /* Lê e cria as arestas. */
    for (int i = 0; i < qtdArestas; i++) {
        int origem;
        int destino;

        if (fscanf(arquivo, "%d %d", &origem, &destino) != 2) {
            fclose(arquivo);
            GGdestroiGrafo(grafo);
            return NULL;
        }

        if (GAcriaAresta(grafo, origem, destino) == NULL) {
            fclose(arquivo);
            GGdestroiGrafo(grafo);
            return NULL;
        }
    }

    fclose(arquivo);

    return grafo;
}

int GBsalvaGrafo(Grafo* grafo, char* nomeArquivo) {
    if (grafo == NULL || nomeArquivo == NULL) {
        return 0;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        return 0;
    }

    /* Escreve a quantidade de vértices e arestas. */
    if (fprintf(arquivo, "%d %d\n",
                grafo->qtdVertices,
                grafo->qtdArestas) < 0) {
        fclose(arquivo);
        return 0;
    }

    /* Escreve os extremos de cada aresta. */
    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (fprintf(arquivo, "%d %d\n",
                    aresta->alfa,
                    aresta->omega) < 0) {
            fclose(arquivo);
            return 0;
        }
    }

    /* Verifica se houve erro ao fechar o arquivo. */
    if (fclose(arquivo) != 0) {
        return 0;
    }

    return 1;
}

int GBsalvaGrafoDot(Grafo* grafo, char* nomeArquivo) {
    if (grafo == NULL || nomeArquivo == NULL) {
        return 0;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        return 0;
    }

    /* Define o tipo de grafo no arquivo DOT. */
    if (grafo->dirigido) {
        fprintf(arquivo, "digraph G {\n");
    } else {
        fprintf(arquivo, "graph G {\n");
    }

    /* Declara todos os vértices. */
    for (int i = 0; i < grafo->qtdVertices; i++) {
        fprintf(arquivo, "    %d;\n", grafo->vertices[i]->id);
    }

    /* Escreve as arestas. */
    for (int i = 0; i < grafo->qtdArestas; i++) {
        Aresta* aresta = grafo->arestas[i];

        if (grafo->dirigido) {
            fprintf(arquivo, "    %d -> %d;\n",
                    aresta->alfa, aresta->omega);
        } else {
            fprintf(arquivo, "    %d -- %d;\n",
                    aresta->alfa, aresta->omega);
        }
    }

    fprintf(arquivo, "}\n");

    if (fclose(arquivo) != 0) {
        return 0;
    }

    return 1;
}

#ifndef GRAFO_SEM_MAIN
int main(void) {
    Grafo* grafo = GGcriaGrafoTipo(5, 10, 1);

    if (grafo == NULL) {
        printf("Erro ao criar grafo.\n");
        return 1;
    }

    Vertice* v0 = GVcriaVertice(grafo);
    Vertice* v1 = GVcriaVertice(grafo);
    Vertice* v2 = GVcriaVertice(grafo);

    if (v0 == NULL || v1 == NULL || v2 == NULL) {
        printf("Erro ao criar vertices.\n");
        GGdestroiGrafo(grafo);
        return 1;
    }

    /* Cria arestas entre os IDs 0, 1 e 2. */
    Aresta* a0 = GAcriaAresta(grafo, 0, 1);
    Aresta* a1 = GAcriaAresta(grafo, 1, 2);

    if (a0 == NULL || a1 == NULL) {
        printf("Erro ao criar arestas.\n");
        GGdestroiGrafo(grafo);
        return 1;
    }

    printf("Aresta 0 existe? %d\n",
           GBexisteIdAresta(grafo, 0));

    printf("Aresta 5 existe? %d\n",
           GBexisteIdAresta(grafo, 5));

    printf("Existe aresta de 0 para 1? %d\n",
           GBexisteArestaDir(grafo, 0, 1));

    printf("Existe aresta de 1 para 0? %d\n",
           GBexisteArestaDir(grafo, 1, 0));

    printf("Primeira aresta: %d\n",
           GAprimeiraAresta(grafo));

    printf("Proxima aresta apos 0: %d\n",
           GAproximaAresta(grafo, 0));

    printf("Proxima aresta apos 1: %d\n",
           GAproximaAresta(grafo, 1));

    GGdestroiGrafo(grafo);

    return 0;
}
#endif
