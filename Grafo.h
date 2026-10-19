#pragma once

typedef struct aresta Aresta;
typedef struct vertice Vertice;
typedef struct grafo Grafo;

/* Criação e destruição */

int GehDirigido(Grafo* grafo);

Grafo* GGcriaGrafo(int v, int a);

/*
 * Extensão da interface original:
 * permite criar grafos dirigidos e não dirigidos.
 *
 * dirigido = 1: dirigido
 * dirigido = 0: não dirigido
 */
Grafo* GGcriaGrafoTipo(int v, int a, int dirigido);

void GGdestroiGrafo(Grafo* grafo);

/* Vértices */

Vertice* GVcriaVertice(Grafo* grafo);

int GBexisteIdVertice(Grafo* grafo, int id);

int GVprimeiroVertice(Grafo* grafo);

int GVproximoVertice(Grafo* grafo, int v1);

/* Arestas */

int GBexisteIdAresta(Grafo* grafo, int id);

Aresta* GAcriaAresta(Grafo* grafo, int v1, int v2);

int GBexisteArestaDir(Grafo* grafo, int v1, int v2);

int GBexisteAresta(Grafo* grafo, int v1, int v2);

Aresta* GApegaArestaDir(Grafo* grafo, int v1, int v2);

Aresta* GApegaAresta(Grafo* grafo, int v1, int v2);

int GAprimeiraAresta(Grafo* grafo);

int GAproximaAresta(Grafo* grafo, int a1);

/* Quantidades */

int GInumeroVertices(Grafo* grafo);

int GInumeroVerticesMax(Grafo* grafo);

int GInumeroArestas(Grafo* grafo);

int GInumeroArestasMax(Grafo* grafo);

/* Arquivos */

Grafo* GGcarregaGrafo(char* f);

int GBsalvaGrafo(Grafo* grafo, char* nomeArquivo);

int GBsalvaGrafoDot(Grafo* grafo, char* nomeArquivo);

/* Incidências */

int GIpegaGrau(Grafo* grafo, int v);

int GAprimaAresta(Grafo* grafo, int v);

int GAproxAresta(Grafo* grafo, int v, int a1);

int GAprimaEntrada(Grafo* grafo, int v);

int GAproxEntrada(Grafo* grafo, int v, int a1);

int GAprimaSaida(Grafo* grafo, int v);

int GAproxSaida(Grafo* grafo, int v, int a1);

/* Propriedades das arestas */

int GBarestaLaco(Grafo* grafo, int a);

int GValfa(Grafo* grafo, int a);

int GVomega(Grafo* grafo, int a);

int GVvizinho(Grafo* grafo, int v, int a);