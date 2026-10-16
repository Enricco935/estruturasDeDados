#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct aresta
{
    int id;
    int idVerticePartida;
    int idVerticeChegada;
}Aresta;

typedef struct vertice
{
    int id;
    int qtdArestas;
    int* listaIdArestas;
}Vertice;

typedef struct grafo
{
    Aresta* listaAresta;
    Vertice* listaVertice;

    int qtdArestas;
    int qtdVertices;

    int MaxArestas;
    int MaxVertices;

    int ultIdVertice;
    int ultIdAresta;

    int eDirigido;
} Grafo;

const Vertice VERTICE_ERRO = {-1, -1, NULL};
const Aresta ARESTA_ERRO = {-1, -1, -1};

// Funcoes uteis

int existeGrafo(Grafo* grafo) {
    return (grafo && grafo->listaVertice && grafo->listaAresta);
}


Vertice* buscaVerticePorId(Grafo* grafo, int id) {
    if(!existeGrafo(grafo))
        return NULL;

    for (int i = 0; i < grafo->qtdVertices; i++)
    {
        Vertice* vertice = &grafo->listaVertice[i];
        if (id == vertice->id)
            return vertice; 
    }
    
    return NULL;
}

int insereIdArestaNaListaDoVertice(Vertice* vertice, int idAresta, int qtdMaxArestas) {

    if (vertice == NULL || vertice->listaIdArestas == NULL)
        return 0;

    for (int i = 0; i < vertice->qtdArestas; i++) 
    {
        int id = vertice->listaIdArestas[i];
        if (id == idAresta)
            return 1;
    }

    if (vertice->qtdArestas >= qtdMaxArestas)
        return 0;
    
    
    vertice->listaIdArestas[vertice->qtdArestas++] = idAresta;
    return 1;
}

int menorIdVertice(Vertice* listaVertice, int tamanhoLista, int piso) {

    if (listaVertice == NULL || tamanhoLista <= 0) {
        return -1;
    }

    int menor = -1;

    for (int i = 0; i < tamanhoLista; i++) {
        int idAtual = listaVertice[i].id;

        if (idAtual >= piso) {
            if (menor == -1 || idAtual < menor) {
                menor = idAtual;
            }
        }
    }

    return menor;
}

int existeVertice(Vertice* listaVertice, int tamanhoLista, int id) {
    if (listaVertice == NULL || tamanhoLista <= 0) {
        return 0;
    }

    for (int i = 0; i < tamanhoLista; i++)
    {
        Vertice vertice = listaVertice[i];
        if (id == vertice.id)
            return 1; 
    }
    
    return 0;
}

Aresta criaAresta(int id, int idPartida, int idChegada) {
    Aresta novaAresta;
    novaAresta.id = id;
    novaAresta.idVerticePartida = idPartida;
    novaAresta.idVerticeChegada = idChegada;

    return novaAresta;
}

Vertice criaVertice(int id) {
    Vertice novoVertice;
    novoVertice.id = id;
    novoVertice.listaIdArestas = NULL;
    novoVertice.qtdArestas = 0;

    return novoVertice;
}

void ordenaListaVertices(Vertice* listaVertice, int qtdVertices) {
    for (int i = 0; i < qtdVertices - 1; i++) {
        for (int j = 0; j < qtdVertices - i - 1; j++) {
            if (listaVertice[j].id > listaVertice[j + 1].id) {
                Vertice aux = listaVertice[j];
                listaVertice[j] = listaVertice[j + 1];
                listaVertice[j + 1] = aux;
            }
        }
    }
}

Vertice criaVerticeNoGrafoComId(Grafo* grafo, int id) {
    Vertice novoVertice;
    novoVertice.id = id;
    novoVertice.qtdArestas = 0;
    novoVertice.listaIdArestas = malloc(sizeof(int) * grafo->MaxArestas);

    if (grafo->qtdVertices >= grafo->MaxVertices)
        return VERTICE_ERRO;

    grafo->listaVertice[grafo->qtdVertices++] = novoVertice;

    return novoVertice;
}
// Funcoes da implementação 

Grafo* GGcriaGrafo(int v, int a) {
    Grafo* novoGrafo = malloc(sizeof(Grafo));

    if (!novoGrafo)
        return NULL;

    novoGrafo->eDirigido = 1;

    novoGrafo->qtdVertices = 0;
    novoGrafo->qtdArestas = 0;

    novoGrafo->MaxVertices = v;
    novoGrafo->MaxArestas = a;
    
    novoGrafo->ultIdVertice = 0;
    novoGrafo->ultIdAresta = 0;
    
    novoGrafo->listaVertice = malloc(sizeof(Vertice) * v);
    novoGrafo->listaAresta = malloc(sizeof(Aresta) * a);
    
    if (!novoGrafo->listaAresta || !novoGrafo->listaVertice)
        return NULL;
    
    return novoGrafo;
}

void GGdestroiGrafo(Grafo* grafo) {
    if (!grafo || !grafo->listaAresta || !grafo->listaVertice)
        return;

    free(grafo->listaAresta);
    free(grafo->listaVertice);
    free(grafo);
}

Vertice GVcriaVertice(Grafo* grafo) {
    Vertice novoVertice;
    novoVertice.id = ++grafo->ultIdVertice;
    novoVertice.qtdArestas = 0;
    novoVertice.listaIdArestas = malloc(sizeof(int) * grafo->MaxArestas);

    if (grafo->qtdVertices >= grafo->MaxVertices)
        return VERTICE_ERRO;

    grafo->listaVertice[grafo->qtdVertices++] = novoVertice;

    return novoVertice;
}

int GBexisteIdVertice(Grafo* grafo, int id) {
    if(!existeGrafo(grafo))
        return 0;

    for (int i = 0; i < grafo->qtdVertices; i++)
    {
        Vertice vertice = grafo->listaVertice[i];
        if (id == vertice.id)
            return 1; 
    }
    
    return 0;
}

int GBexisteIdAresta(Grafo* grafo, int id) {
    if(!existeGrafo(grafo))
        return 0;

    for (int i = 0; i < grafo->qtdArestas; i++)
    {
        Aresta aresta = grafo->listaAresta[i];
        if (id == aresta.id)
            return 1; 
    }
    
    return 0;
}

Aresta GAcriaAresta(Grafo* grafo, int v1, int v2) {
    if (!GBexisteIdVertice(grafo, v1) || !GBexisteIdVertice(grafo, v2))
        return ARESTA_ERRO;
    
    if (grafo->qtdArestas >= grafo->MaxArestas)
        return ARESTA_ERRO;

    Aresta novaAresta;
    Vertice* verticePartida = buscaVerticePorId(grafo, v1);
    Vertice* verticeChegada = buscaVerticePorId(grafo, v2);


    novaAresta.id = ++grafo->ultIdAresta;
    novaAresta.idVerticePartida = v1;
    novaAresta.idVerticeChegada = v2;
    grafo->listaAresta[grafo->qtdArestas++] = novaAresta;
    insereIdArestaNaListaDoVertice(verticePartida, novaAresta.id, grafo->MaxArestas);
    insereIdArestaNaListaDoVertice(verticeChegada, novaAresta.id, grafo->MaxArestas);

    return novaAresta;
}

int GBexisteArestaDir(Grafo* grafo, int v1, int v2) {
    if (!existeGrafo(grafo))
        return 0;

    if (!GBexisteIdVertice(grafo, v1) ||
        !GBexisteIdVertice(grafo, v2))
        return 0;

    for (int i = 0; i < grafo->qtdArestas; i++)
    {
        Aresta aresta = grafo->listaAresta[i];

        if (aresta.idVerticePartida == v1 && aresta.idVerticeChegada == v2)
        {
            return 1;
        }
    }
    
    return 0;
}

int GBexisteAresta(Grafo* grafo, int v1, int v2) {

    if (GBexisteArestaDir(grafo, v1, v2) && GBexisteArestaDir(grafo, v2, v1))
    {
        return 1;
    }
    return 0;
}

//pega o identificador de uma aresta dirigida
Aresta GApegaArestaDir (Grafo* grafo, int v1, int v2) {
    if (!GBexisteArestaDir(grafo, v1, v2))
    {
        return ARESTA_ERRO;
    }

    for (int i = 0; i < grafo->qtdArestas; i++)
    {
        Aresta aresta = grafo->listaAresta[i];

        if (aresta.idVerticePartida == v1 && aresta.idVerticeChegada == v2)
        {
            return aresta;
        }
    }

    return ARESTA_ERRO;
}

//pega o identificador de uma aresta não dirigida
Aresta GApegaAresta(Grafo* grafo, int v1, int v2) {
    if (!GBexisteAresta(grafo, v1, v2))
    {
        return ARESTA_ERRO;
    }

    for (int i = 0; i < grafo->qtdArestas; i++)
    {
        Aresta aresta = grafo->listaAresta[i];

        if (aresta.idVerticePartida == v1 && aresta.idVerticeChegada == v2)
        {
            if (aresta.idVerticePartida == v2 && aresta.idVerticeChegada == v1)
            {
                return aresta;
            }            
        }
    }

    return ARESTA_ERRO;
}

//pega o primeiro vértice do grafo
int GVprimeiroVertice(Grafo* grafo) {
    if (!existeGrafo(grafo) || grafo->qtdVertices == 0)
    {
        return -1;
    }
    
    return menorIdVertice(grafo->listaVertice, grafo->qtdVertices, -1);
}

//pega o próximo vértice do grafo
int GVproximoVertice(Grafo* grafo, int v1){
    if (!existeGrafo(grafo) || grafo->qtdVertices == 0)
    {
        return -1;
    }

    return menorIdVertice(grafo->listaVertice, grafo->qtdVertices, v1+1);
}

//pega a primeira aresta do grafo
int GAprimeiraAresta(Grafo* grafo) {
    if (!existeGrafo(grafo) || grafo->qtdArestas == 0) {
        return -1;
    }

    int menor = grafo->listaAresta[0].id;

    for (int i = 1; i < grafo->qtdArestas; i++) {
        int idAtual = grafo->listaAresta[i].id;

        if (idAtual < menor) {
            menor = idAtual;
        }
    }

    return menor;
}

// Pega a próxima aresta do grafo
int GAproximaAresta(Grafo* grafo, int a1) {
    if (!existeGrafo(grafo) || grafo->qtdArestas == 0) {
        return -1;
    }

    int menor = -1;

    for (int i = 0; i < grafo->qtdArestas; i++) {
        int idAtual = grafo->listaAresta[i].id;

        if (idAtual > a1) {
            if (menor == -1 || idAtual < menor) {
                menor = idAtual;
            }
        }
    }

    return menor;
}

// pega número de vértices no grafo
int GInumeroVertices(Grafo* grafo){
    if (!existeGrafo(grafo)) {
        return -1;
    }

    return grafo->qtdVertices;
}

// Retorna o número máximo de vértices do grafo
int GInumeroVerticesMax(Grafo* grafo) {
    if (!existeGrafo(grafo)) {
        return -1;
    }

    return grafo->MaxVertices;
}

// Retorna o número de arestas do grafo
int GInumeroArestas(Grafo* grafo) {
    if (!existeGrafo(grafo)) {
        return -1;
    }

    return grafo->qtdArestas;
}

// Retorna o número máximo de arestas do grafo
int GInumeroArestasMax(Grafo* grafo) {
    if (!existeGrafo(grafo)) {
        return -1;
    }

    return grafo->MaxArestas;
}

// cria e carrega um grafo armazenado em arquivo
Grafo* GGcarregaGrafo(char* f) {
    FILE *arquivo = fopen(f, "r");

    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    char linha[64];
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return NULL;
    }
    char tipoGrafo[20];
    if (sscanf(linha, "%19s", tipoGrafo) != 1) {
        fclose(arquivo);
        return NULL;
    }
    int eDirecionado;
    if (strcmp(tipoGrafo, "graph") == 0) {
        eDirecionado = 0;
    }
    else if (strcmp(tipoGrafo, "digraph") == 0) {
       eDirecionado = 1;
    }
    else {
        printf("Formato de arquivo invalido.\n");
        fclose(arquivo);
        return NULL;
    }

    int qtdMaxVertice = 50;
    int qtdMaxAresta = 50;

    Vertice* verticesArquivo = malloc(sizeof(Vertice) * qtdMaxVertice);
    Aresta* arestasArquivo = malloc(sizeof(Aresta) * qtdMaxAresta);

    int qtdVertice = 0;
    int qtdAresta = 0;

    if (!verticesArquivo || !arestasArquivo)
        return NULL;

    while (fgets(linha, sizeof(linha), arquivo)) {

        if(strcmp(linha, "{") == 0) continue;
        if(strcmp(linha, "}") == 0) continue;

        if (qtdAresta + 2 >= qtdMaxAresta) {
            qtdMaxAresta = qtdMaxAresta * 2;
            arestasArquivo = realloc(arestasArquivo, sizeof(Aresta) * qtdMaxAresta);
        }
        if (qtdVertice + 2 >= qtdMaxVertice) {
            qtdMaxVertice = qtdMaxVertice * 2;
            verticesArquivo = realloc(verticesArquivo, sizeof(Vertice) * qtdMaxVertice);
        }

        int origem, destino;

        if (eDirecionado)
        {
            if (sscanf(linha, "%d -> %d;", &origem, &destino) == 2) {
                
                arestasArquivo[qtdAresta++] = criaAresta(qtdAresta + 1, origem, destino);

                if (!existeVertice(verticesArquivo, qtdVertice, origem)) {    
                    verticesArquivo[qtdVertice++] = criaVertice(origem);
                }
                if (!existeVertice(verticesArquivo, qtdVertice, destino)) {
                    verticesArquivo[qtdVertice++] = criaVertice(destino);
                }
            }
        }
        else
        {
            if (sscanf(linha, "%d -- %d;", &origem, &destino) == 2) {

                arestasArquivo[qtdAresta++] = criaAresta(qtdAresta + 1, origem, destino);
                arestasArquivo[qtdAresta++] = criaAresta(qtdAresta + 1, destino, origem);

                if (!existeVertice(verticesArquivo, qtdVertice, origem)) {    
                    verticesArquivo[qtdVertice++] = criaVertice(origem);
                }
                if (!existeVertice(verticesArquivo, qtdVertice, destino)) {
                    verticesArquivo[qtdVertice++] = criaVertice(destino);
                }
            }
        }
        
    }

    

    Grafo* novoGrafo = GGcriaGrafo(qtdVertice, qtdAresta);

    novoGrafo->eDirigido = eDirecionado;

    for (int i = 0; i < qtdVertice; i++)
    {
        Vertice vertice = verticesArquivo[i];
        criaVerticeNoGrafoComId(novoGrafo, vertice.id);
    }

    for (int i = 0; i < qtdAresta; i++)
    {
        Aresta aresta = arestasArquivo[i];
        GAcriaAresta(novoGrafo, aresta.idVerticePartida, aresta.idVerticeChegada);
        
    }

    free(arestasArquivo);
    free(verticesArquivo);
    
    fclose(arquivo);

    return novoGrafo;
}

// salva o grafo em arquivo
int GBsalvaGrafo(Grafo* grafo, char* nomeArquivo) {
    if (grafo == NULL || nomeArquivo == NULL) {
        return 0;
    }

    FILE* arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        perror("Erro ao criar arquivo");
        return 0;
    }

    // Define o tipo do grafo
    if (grafo->eDirigido) {
        fprintf(arquivo, "digraph G {\n");
    } else {
        fprintf(arquivo, "graph G {\n");
    }

    // Salva os vertices
    for (int i = 0; i < grafo->qtdVertices; i++) {
        fprintf(arquivo, "    %d;\n", grafo->listaVertice[i].id);
    }

    // Salva as arestas
    for (int i = 0; i < grafo->qtdArestas; i++) {
        int origem = grafo->listaAresta[i].idVerticePartida;
        int destino = grafo->listaAresta[i].idVerticeChegada;

        if (grafo->eDirigido) {
            fprintf(arquivo, "    %d -> %d;\n", origem, destino);
        } else {
            // Evita salvar a mesma ligação duas vezes
            if (origem <= destino) {
                fprintf(arquivo, "    %d -- %d;\n", origem, destino);
            }
        }
    }

    fprintf(arquivo, "}\n");

    fclose(arquivo);

    return 1;
}

//pega o grau do vértice
int GIpegaGrau(Grafo* grafo, int v) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    int qtdArestas = buscaVerticePorId(grafo, v)->qtdArestas;

    if (!grafo->eDirigido){
        return qtdArestas / 2;
    }
    
    return qtdArestas;
} 

//pega primeira aresta na estrela do vértice, em grafos não dirigidos
int GAprimaAresta(Grafo* grafo, int v) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);

    if (vertice->qtdArestas == 0) {
        return -1;
    }

    return vertice->listaIdArestas[0];
}

// pega próxima aresta na estrela do vértice, em grafos não dirigidos
int GAproxAresta(Grafo* grafo, int v, int a1) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);

    for (int i = 0; i < vertice->qtdArestas; i+=2) {
        if (vertice->listaIdArestas[i] == a1) {
            if (i + 2 < vertice->qtdArestas) {
                return vertice->listaIdArestas[i + 2];
            }

            return -1;
        }
    }

    return -1;
}

// pega primeira aresta na estrela de entrada do vértice, em grafos dirigidos
int GAprimaEntrada(Grafo* grafo, int v) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);

    for (int i = 0; i < vertice->qtdArestas; i++) {
        int idAresta = vertice->listaIdArestas[i];

        for (int j = 0; j < grafo->qtdArestas; j++) {
            if (grafo->listaAresta[j].id == idAresta &&
                grafo->listaAresta[j].idVerticeChegada == v) {
                return idAresta;
            }
        }
    }

    return -1;
}

// pega próxima aresta na estrela de entrada do vértice, em grafos dirigidos
int GAproxEntrada(Grafo* grafo, int v, int a1) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);
    int encontrouArestaAnterior = 0;

    for (int i = 0; i < vertice->qtdArestas; i++) {
        int idAresta = vertice->listaIdArestas[i];

        for (int j = 0; j < grafo->qtdArestas; j++) {
            if (grafo->listaAresta[j].id == idAresta &&
                grafo->listaAresta[j].idVerticeChegada == v) {

                if (encontrouArestaAnterior) {
                    return idAresta;
                }

                if (idAresta == a1) {
                    encontrouArestaAnterior = 1;
                }
            }
        }
    }

    return -1;
}

// Pega primeira aresta na estrela de saída do vértice, em grafos dirigidos
int GAprimaSaida(Grafo* grafo, int v) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);

    for (int i = 0; i < vertice->qtdArestas; i++) {
        int idAresta = vertice->listaIdArestas[i];

        for (int j = 0; j < grafo->qtdArestas; j++) {
            if (grafo->listaAresta[j].id == idAresta &&
                grafo->listaAresta[j].idVerticePartida == v) {
                return idAresta;
            }
        }
    }

    return -1;
}

// Pega próxima aresta na estrela de saída do vértice, em grafos dirigidos
int GAproxSaida(Grafo* grafo, int v, int a1) {
    if (!GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    Vertice* vertice = buscaVerticePorId(grafo, v);
    int encontrouAresta = 0;

    for (int i = 0; i < vertice->qtdArestas; i++) {
        int idAresta = vertice->listaIdArestas[i];

        for (int j = 0; j < grafo->qtdArestas; j++) {
            if (grafo->listaAresta[j].id == idAresta &&
                grafo->listaAresta[j].idVerticePartida == v) {

                if (encontrouAresta) {
                    return idAresta;
                }

                if (idAresta == a1) {
                    encontrouAresta = 1;
                }
            }
        }
    }

    return -1;
}

// Verifica se uma aresta é um laço
int GBarestaLaco(Grafo* grafo, int a) {
    if (grafo == NULL) {
        return 0;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->listaAresta[i].id == a) {
            return grafo->listaAresta[i].idVerticePartida ==
                   grafo->listaAresta[i].idVerticeChegada;
        }
    }

    return 0;
}

// Pega o vértice de partida da aresta
int GValfa(Grafo* grafo, int a) {
    if (grafo == NULL) {
        return -1;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->listaAresta[i].id == a) {
            return grafo->listaAresta[i].idVerticePartida;
        }
    }

    return -1;
}

// Pega o vértice de chegada da aresta
int GVomega(Grafo* grafo, int a) {
    if (grafo == NULL) {
        return -1;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->listaAresta[i].id == a) {
            return grafo->listaAresta[i].idVerticeChegada;
        }
    }

    return -1;
}

// Pega o outro vértice no extremo da aresta
int GVvizinho(Grafo* grafo, int v, int a) {
    if (grafo == NULL || !GBexisteIdVertice(grafo, v)) {
        return -1;
    }

    for (int i = 0; i < grafo->qtdArestas; i++) {
        if (grafo->listaAresta[i].id == a) {
            int origem = grafo->listaAresta[i].idVerticePartida;
            int destino = grafo->listaAresta[i].idVerticeChegada;

            if (origem == v) {
                return destino;
            }

            if (destino == v) {
                return origem;
            }

            return -1;
        }
    }

    return -1;
}

int main(int argc, char const *argv[])
{
    Grafo* grafo = GGcarregaGrafo("grafo2.dot");

    printf("\n%d\n", GIpegaGrau(grafo, 1));
    GBsalvaGrafo(grafo, "salvamento.dot");
    
}







































