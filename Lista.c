#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    void* conteudo;
    struct no* proximo;
    struct no* anterior;
}No;

typedef struct lista
{
    int tamanho;
    No* inicio;
    No* fim;
}Lista;

int Lvazia(Lista* lista);

// Funcoes privadas
No* encontraPosicaoPeloFinal(Lista* lista, int posicao) {
    if (posicao >= lista->tamanho || posicao < 0)
        return NULL;
    
    No* noAtual = lista->fim;
    for (int i = lista->tamanho - 1; i > posicao; i--) 
        noAtual = noAtual->anterior;

    return noAtual;
}

No* encontraPosicaoPeloInicio(Lista* lista, int posicao) {
    if (posicao >= lista->tamanho || posicao < 0)
        return NULL;
    
    No* noAtual = lista->inicio;
    for (int i = 0; i < posicao; i++) 
        noAtual = noAtual->proximo;

    return noAtual;
}

No* encontraNoMenorDistancia(Lista* lista, int posicao) {
    // decidir se é melhor percorrer a partir do fim ou do inicio
    int distanciaInicio = posicao;
    int distanciaFim = lista->tamanho - posicao;

    if (distanciaFim < distanciaInicio)
    {
        return encontraPosicaoPeloFinal(lista, posicao);
    } else {
        return encontraPosicaoPeloInicio(lista, posicao);
    }
}

void insereInicio(Lista* lista, No* novoNo) {
    novoNo->anterior = NULL;
    novoNo->proximo = lista->inicio;
    if (!Lvazia(lista)){
        lista->inicio->anterior = novoNo;
    } else
    {
        lista->fim = novoNo;
    }
    
    lista->inicio = novoNo;
}

void insereFim(Lista* lista, No* novoNo) {
    novoNo->proximo = NULL;
    novoNo->anterior = lista->fim;
    if (!Lvazia(lista)){
        lista->fim->proximo = novoNo;

    } else
    {
        lista->inicio = novoNo;
    }
    lista->fim = novoNo;
}

void insereMeio(Lista* lista, No* novoNo, No* noAtual) {
    novoNo->proximo = noAtual;
    novoNo->anterior = noAtual->anterior;
    noAtual->anterior->proximo = novoNo;
    noAtual->anterior = novoNo;
}

void removeInicio(Lista* lista) {
    No* noRemovido = lista->inicio;
    lista->inicio = noRemovido->proximo;

    noRemovido->proximo = NULL;
    free(noRemovido);
    if (!Lvazia(lista)){
        lista->inicio->anterior = NULL;
    } else {
        lista->fim = NULL;
    }
}

void removeFim(Lista* lista) {
    No* noRemovido = lista->fim;
    lista->fim = noRemovido->anterior;

    noRemovido->anterior = NULL;
    free(noRemovido);
    if (!Lvazia(lista)){
        lista->fim->proximo = NULL;
    } else {
        lista->inicio = NULL;
    }
}

void removeMeio(No* noRemovido) {
    noRemovido->anterior->proximo = noRemovido->proximo;
    noRemovido->proximo->anterior = noRemovido->anterior;
    
    noRemovido->anterior = NULL;
    noRemovido->proximo = NULL;
    free(noRemovido);
}

// Funcoes publicas
Lista* Lcria() {
    Lista* lista = malloc(sizeof(Lista));

    if (!lista)
        return NULL;

    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;

    return lista;
}

void Ldestroi(Lista** lista){
    while (!Lvazia(*lista))
    {
        removeFim(*lista);
    }

    free(*lista);
}

int Linsere(Lista* lista, void* conteudo, int posicao) {
    if (posicao > lista->tamanho)
        return 0;    

    // criar o no e colocar o conteudo nele
    No* novoNo = malloc(sizeof(No));
    novoNo->conteudo = conteudo;

    if (posicao == 0) {
        insereInicio(lista, novoNo);
    }
    else if (posicao == lista->tamanho) {
        insereFim(lista, novoNo);
    }
    else {
        insereMeio(lista, novoNo, encontraNoMenorDistancia(lista, posicao));
    }
    
    lista->tamanho++;
    return 1;
}

int Lvazia(Lista* lista) {

    return (!lista->fim || !lista->inicio);
}

int Lcheia() {
    return 0;
}

int Ltamanho(Lista* lista) {
    return lista->tamanho;
}

void* Lexamina(Lista* lista, int posicao) {
    if (posicao >= lista->tamanho || posicao < 0)
        return NULL;

    return encontraNoMenorDistancia(lista, posicao)->conteudo;
}

int Llocaliza(Lista* lista, void* elemento, int (*funcaoComparacao)(void* a, void* b)) {
    int posicao = 0;

    No* noAtual = lista->inicio;
    while (noAtual != 0)
    {
        if (funcaoComparacao(noAtual->conteudo, elemento) == 1)
            return posicao;
        
        noAtual = noAtual->proximo;
        posicao++;
    }
    
    return -1;
}

void* Lremove(Lista* lista, int posicao) {
    if (posicao >= lista->tamanho || posicao < 0)
        return NULL;
    
    No* noRemovido = encontraNoMenorDistancia(lista, posicao);
    void* conteudo = noRemovido->conteudo;

    if (posicao == 0) {
        removeInicio(lista);
    }
    else if (posicao == lista->tamanho - 1) {
        removeFim(lista);
    }
    else {
        removeMeio(noRemovido);
    }

    lista->tamanho--;
    return conteudo;
}
