// Implementação de TADs simples e grafo

// Alunos:
// Enricco Augusto Rodrigues Faria  | RA: 0117036
// João Vitor Da Silva Silverio     | RA: 0117181
// 19/10/2026

// Professor:
// Walace de Almeida Rodrigues

#include <stdio.h>
#include <stdlib.h>

typedef struct no{
    void* conteudo;
    struct no* proximo;
} No;

typedef struct fila{
    No* in;
    No* out;
    int tamanho;
} Fila;

Fila* Fcria() {
    Fila* fila = malloc(sizeof(Fila));
    if (!fila)
        return NULL;

    fila->in = NULL;
    fila->out = NULL;
    fila->tamanho = 0;

    return fila;
}

int Fvazia(Fila* fila) {
    return (fila->in == NULL || fila->out == NULL);
}

int Fcheia() {
    return 0;
}

int Ftamanho(Fila* fila) {

    return fila->tamanho;
}

void* Fexamina(Fila* fila) {
    if (Fvazia(fila))
        return NULL;

    return fila->out->conteudo;
}

void Finsere(Fila* fila, void* conteudo) {
    
    No* novoNo = malloc(sizeof(No));
    novoNo->conteudo = conteudo;
    novoNo->proximo = NULL;

    if (!Fvazia(fila))
    {
        fila->in->proximo = novoNo;
    } else {
        fila->out = novoNo;
    }
    fila->in = novoNo;
    fila->tamanho++;
}

void* Fremove(Fila* fila) {
    if (Fvazia(fila))
        return NULL;

    No* noRemovido = fila->out;
    void* conteudo = noRemovido->conteudo;

    fila->out = noRemovido->proximo;

    if (fila->out == NULL)
        fila->in = NULL;

    free(noRemovido);
    fila->tamanho--;

    return conteudo;
}

void Fdestroi(Fila** fila) {
    while (!Fvazia(*fila))
        Fremove(*fila);

    free(*fila);
}