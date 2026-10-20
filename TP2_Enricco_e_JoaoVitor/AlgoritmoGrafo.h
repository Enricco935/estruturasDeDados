#pragma once

#include "Grafo.h"

void GbuscaLargura(Grafo *grafo, int inicio);

void GbuscaProfundidade(Grafo *grafo, int inicio);

void GcaminhoMaisCurto(Grafo *grafo, const float *pesos,
                       int inicio, int chegada);

void GcomponentesConexos(Grafo *grafo);

void Gkruskal(Grafo *grafo, const float *pesos);
