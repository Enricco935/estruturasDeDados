#pragma once

typedef struct pilha Pilha;

Pilha* Pcria();

void Pdestroi(Pilha** pilha);

int Pvazia(Pilha* pilha);

int Pcheia();

int Ptamanho(Pilha* pilha);

void* Pexamina(Pilha* pilha);

void Pinsere(Pilha* pilha, void* conteudo);

void* Premove(Pilha* pilha);