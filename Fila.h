#pragma once

typedef struct fila Fila;

Fila* Fcria();

void Fdestroi(Fila** fila);

int Fvazia(Fila* fila);

int Fcheia();

int Ftamanho(Fila* fila);

void* Fexamina(Fila* fila);

void Finsere(Fila* fila, void* conteudo);

void* Fremove(Fila* fila);