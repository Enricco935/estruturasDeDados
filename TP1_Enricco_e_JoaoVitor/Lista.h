#pragma once

typedef struct lista Lista;

Lista* Lcria();

void Ldestroi(Lista** lista); 

int Linsere(Lista* lista, void* conteudo, int posicao);

int Lvazia(Lista* lista);

int Lcheia();

int Ltamanho(Lista* lista);

void* Lexamina(Lista* lista, int posicao);

int Llocaliza(Lista* lista, void* elemento, int (*funcaoComparacao)(void* a, void* b));

void* Lremove(Lista* lista, int posicao);