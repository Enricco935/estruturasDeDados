#include <stdlib.h>
#include <stdio.h>


typedef struct no
{
    void* conteudo;
    struct no* anterior;
} No;

typedef struct pilha
{
    No* topo;
    int tamanho;
}Pilha;

int Pvazia(Pilha* pilha) {
    return pilha->topo == NULL;
}

int Pcheia() {
    return 0;
}

int Ptamanho(Pilha* pilha){
    return pilha->tamanho;
}

Pilha* Pcria() {

    //aloca memoria para a pilha
    Pilha* pilha = malloc(sizeof(Pilha));

    //aponta o primeiro no pra null
    pilha->topo = NULL;
    pilha->tamanho = 0;

    return pilha;
}

void Pinsere(Pilha* pilha, void* conteudo) {

    No* novoNo = malloc(sizeof(No));
    novoNo->conteudo = conteudo;

    novoNo->anterior = pilha->topo;
    
    pilha->topo = novoNo;
    pilha->tamanho++;
}

void* Premove(Pilha* pilha) {

    if (Pvazia(pilha))
        return 0;
    
    void* conteudo = pilha->topo->conteudo;
    No* noRemovido = pilha->topo;
    
    pilha->topo = pilha->topo->anterior;
    
    noRemovido->anterior = NULL;
    free(noRemovido);

    pilha->tamanho--;
    return conteudo;
}

void* Pexamina(Pilha* pilha){
    return pilha->topo->conteudo;
}

void Pdestroi(Pilha** pilha) {

    while (!Pvazia(*pilha))
        Premove(*pilha);
    free(*pilha);
}
