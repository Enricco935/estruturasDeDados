#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fila.h"
#include "Grafo.h"
#include "Lista.h"
#include "Pilha.h"

static int leInteiro(const char *mensagem, int *valor) {
    char linha[128];

    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    errno = 0;
    char *fim;
    long numero = strtol(linha, &fim, 10);

    if (fim == linha) {
        printf("Entrada invalida. Informe um numero inteiro.\n");
        return -1;
    }

    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (*fim != '\0' || errno == ERANGE ||
        numero < INT_MIN || numero > INT_MAX) {
        printf("Entrada invalida. Informe um numero inteiro.\n");
        return -1;
    }

    *valor = (int)numero;
    return 1;
}

static int solicitaInteiro(const char *mensagem, int *valor) {
    int resultado;
    do {
        resultado = leInteiro(mensagem, valor);
    } while (resultado < 0);

    return resultado;
}

static int leTexto(const char *mensagem, char *texto, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(texto, tamanho, stdin) == NULL) {
        return 0;
    }

    size_t comprimento = strlen(texto);
    if (comprimento > 0 && texto[comprimento - 1] == '\n') {
        texto[comprimento - 1] = '\0';
    } else {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
    }

    if (texto[0] == '\0') {
        printf("O nome do arquivo nao pode ser vazio.\n");
        return -1;
    }

    return 1;
}

static int solicitaTexto(const char *mensagem, char *texto, size_t tamanho) {
    int resultado;
    do {
        resultado = leTexto(mensagem, texto, tamanho);
    } while (resultado < 0);

    return resultado;
}

static int *criaConteudo(int valor) {
    int *conteudo = malloc(sizeof(*conteudo));
    if (conteudo != NULL) {
        *conteudo = valor;
    }
    return conteudo;
}

static int comparaInteiros(void *a, void *b) {
    return *(int *)a == *(int *)b;
}

static void liberaPilha(Pilha *pilha) {
    while (!Pvazia(pilha)) {
        free(Premove(pilha));
    }
    Pdestroi(&pilha);
}

static void liberaFila(Fila *fila) {
    while (!Fvazia(fila)) {
        free(Fremove(fila));
    }
    Fdestroi(&fila);
}

static void liberaLista(Lista *lista) {
    while (!Lvazia(lista)) {
        free(Lremove(lista, 0));
    }
    Ldestroi(&lista);
}

static void menuPilha(void) {
    Pilha *pilha = Pcria();
    if (pilha == NULL) {
        printf("Nao foi possivel criar a pilha.\n");
        return;
    }

    int opcao;
    for (;;) {
        printf("\n--- Teste da Pilha (LIFO) ---\n"
               "1 - Empilhar valor\n"
               "2 - Desempilhar valor\n"
               "3 - Consultar topo\n"
               "4 - Mostrar tamanho e estado\n"
               "0 - Voltar ao menu principal\n");
        if (!solicitaInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            int valor;
            if (!solicitaInteiro("Valor: ", &valor)) {
                break;
            }

            int *conteudo = criaConteudo(valor);
            if (conteudo == NULL) {
                printf("Erro ao alocar memoria para o valor.\n");
                continue;
            }
            Pinsere(pilha, conteudo);
            printf("Valor %d empilhado.\n", valor);
        } else if (opcao == 2) {
            if (Pvazia(pilha)) {
                printf("A pilha esta vazia.\n");
            } else {
                int *valor = Premove(pilha);
                printf("Valor desempilhado: %d\n", *valor);
                free(valor);
            }
        } else if (opcao == 3) {
            if (Pvazia(pilha)) {
                printf("A pilha esta vazia.\n");
            } else {
                printf("Topo: %d\n", *(int *)Pexamina(pilha));
            }
        } else if (opcao == 4) {
            printf("Tamanho: %d | Vazia: %s | Cheia: %s\n",
                   Ptamanho(pilha), Pvazia(pilha) ? "sim" : "nao",
                   Pcheia() ? "sim" : "nao");
        } else {
            printf("Opcao invalida.\n");
        }
    }

    liberaPilha(pilha);
}

static void menuFila(void) {
    Fila *fila = Fcria();
    if (fila == NULL) {
        printf("Nao foi possivel criar a fila.\n");
        return;
    }

    int opcao;
    for (;;) {
        printf("\n--- Teste da Fila (FIFO) ---\n"
               "1 - Enfileirar valor\n"
               "2 - Desenfileirar valor\n"
               "3 - Consultar primeiro da fila\n"
               "4 - Mostrar tamanho e estado\n"
               "0 - Voltar ao menu principal\n");
        if (!solicitaInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            int valor;
            if (!solicitaInteiro("Valor: ", &valor)) {
                break;
            }

            int *conteudo = criaConteudo(valor);
            if (conteudo == NULL) {
                printf("Erro ao alocar memoria para o valor.\n");
                continue;
            }
            Finsere(fila, conteudo);
            printf("Valor %d enfileirado.\n", valor);
        } else if (opcao == 2) {
            if (Fvazia(fila)) {
                printf("A fila esta vazia.\n");
            } else {
                int *valor = Fremove(fila);
                printf("Valor desenfileirado: %d\n", *valor);
                free(valor);
            }
        } else if (opcao == 3) {
            if (Fvazia(fila)) {
                printf("A fila esta vazia.\n");
            } else {
                printf("Primeiro da fila: %d\n", *(int *)Fexamina(fila));
            }
        } else if (opcao == 4) {
            printf("Tamanho: %d | Vazia: %s | Cheia: %s\n",
                   Ftamanho(fila), Fvazia(fila) ? "sim" : "nao",
                   Fcheia() ? "sim" : "nao");
        } else {
            printf("Opcao invalida.\n");
        }
    }

    liberaFila(fila);
}

static void imprimeLista(Lista *lista) {
    if (Lvazia(lista)) {
        printf("A lista esta vazia.\n");
        return;
    }

    printf("Lista: ");
    for (int i = 0; i < Ltamanho(lista); i++) {
        printf("%s%d", i == 0 ? "" : " <-> ",
               *(int *)Lexamina(lista, i));
    }
    printf("\n");
}

static void menuLista(void) {
    Lista *lista = Lcria();
    if (lista == NULL) {
        printf("Nao foi possivel criar a lista.\n");
        return;
    }

    int opcao;
    for (;;) {
        printf("\n--- Teste da Lista Duplamente Encadeada ---\n"
               "1 - Inserir valor em uma posicao\n"
               "2 - Remover valor de uma posicao\n"
               "3 - Consultar valor em uma posicao\n"
               "4 - Localizar um valor\n"
               "5 - Mostrar lista\n"
               "6 - Mostrar tamanho e estado\n"
               "0 - Voltar ao menu principal\n");
        if (!solicitaInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            int valor;
            int posicao;
            if (!solicitaInteiro("Valor: ", &valor) ||
                !solicitaInteiro("Posicao (0 a tamanho): ", &posicao)) {
                break;
            }
            if (posicao < 0 || posicao > Ltamanho(lista)) {
                printf("Posicao invalida.\n");
                continue;
            }

            int *conteudo = criaConteudo(valor);
            if (conteudo == NULL) {
                printf("Erro ao alocar memoria para o valor.\n");
                continue;
            }
            if (Linsere(lista, conteudo, posicao)) {
                printf("Valor inserido na posicao %d.\n", posicao);
            } else {
                free(conteudo);
                printf("Nao foi possivel inserir o valor.\n");
            }
        } else if (opcao == 2) {
            int posicao;
            if (!solicitaInteiro("Posicao a remover: ", &posicao)) {
                break;
            }
            if (posicao < 0 || posicao >= Ltamanho(lista)) {
                printf("Posicao invalida.\n");
                continue;
            }
            int *valor = Lremove(lista, posicao);
            printf("Valor removido: %d\n", *valor);
            free(valor);
        } else if (opcao == 3) {
            int posicao;
            if (!solicitaInteiro("Posicao a consultar: ", &posicao)) {
                break;
            }
            if (posicao < 0 || posicao >= Ltamanho(lista)) {
                printf("Posicao invalida.\n");
            } else {
                printf("Valor na posicao %d: %d\n",
                       posicao, *(int *)Lexamina(lista, posicao));
            }
        } else if (opcao == 4) {
            int valor;
            if (!solicitaInteiro("Valor a localizar: ", &valor)) {
                break;
            }
            int posicao = Llocaliza(lista, &valor, comparaInteiros);
            if (posicao == -1) {
                printf("Valor nao encontrado.\n");
            } else {
                printf("Valor encontrado na posicao %d.\n", posicao);
            }
        } else if (opcao == 5) {
            imprimeLista(lista);
        } else if (opcao == 6) {
            printf("Tamanho: %d | Vazia: %s | Cheia: %s\n",
                   Ltamanho(lista), Lvazia(lista) ? "sim" : "nao",
                   Lcheia() ? "sim" : "nao");
        } else {
            printf("Opcao invalida.\n");
        }
    }

    liberaLista(lista);
}

static void imprimeArestasDoVertice(Grafo *grafo, int vertice,
                                    const char *tipo,
                                    int primeira, int (*proxima)(Grafo *, int,
                                                                int)) {
    printf("  %s:", tipo);
    int aresta = primeira;
    if (aresta == -1) {
        printf(" nenhuma");
    }
    while (aresta != -1) {
        printf(" a%d(v%d)", aresta, GVvizinho(grafo, vertice, aresta));
        aresta = proxima(grafo, vertice, aresta);
    }
    printf("\n");
}

static void imprimeGrafo(Grafo *grafo) {
    if (grafo == NULL) {
        printf("Crie ou carregue um grafo primeiro.\n");
        return;
    }

    printf("\nGrafo %s | vertices: %d/%d | arestas: %d/%d\n",
           GehDirigido(grafo) ? "dirigido" : "nao dirigido",
           GInumeroVertices(grafo), GInumeroVerticesMax(grafo),
           GInumeroArestas(grafo), GInumeroArestasMax(grafo));

    printf("Vertices:\n");
    int vertice = GVprimeiroVertice(grafo);
    if (vertice == -1) {
        printf("  nenhum\n");
    }
    while (vertice != -1) {
        printf("  v%d (grau %d)\n", vertice, GIpegaGrau(grafo, vertice));
        if (GehDirigido(grafo)) {
            imprimeArestasDoVertice(grafo, vertice, "Entradas",
                                    GAprimaEntrada(grafo, vertice),
                                    GAproxEntrada);
            imprimeArestasDoVertice(grafo, vertice, "Saidas",
                                    GAprimaSaida(grafo, vertice),
                                    GAproxSaida);
        } else {
            imprimeArestasDoVertice(grafo, vertice, "Incidentes",
                                    GAprimaAresta(grafo, vertice),
                                    GAproxAresta);
        }
        vertice = GVproximoVertice(grafo, vertice);
    }

    printf("Arestas:\n");
    int aresta = GAprimeiraAresta(grafo);
    if (aresta == -1) {
        printf("  nenhuma\n");
    }
    while (aresta != -1) {
        int alfa = GValfa(grafo, aresta);
        int omega = GVomega(grafo, aresta);
        printf("  a%d: v%d %s v%d%s\n", aresta, alfa,
               GehDirigido(grafo) ? "->" : "--", omega,
               GBarestaLaco(grafo, aresta) ? " (laco)" : "");
        if (!GBexisteIdAresta(grafo, aresta)) {
            printf("  Erro: a aresta a%d nao foi localizada por ID.\n",
                   aresta);
        }
        aresta = GAproximaAresta(grafo, aresta);
    }
}

static void menuGrafo(void) {
    Grafo *grafo = NULL;
    int opcao;

    for (;;) {
        printf("\n--- Teste da TAD Grafo ---\n"
               "1 - Criar grafo\n"
               "2 - Adicionar vertice\n"
               "3 - Adicionar aresta\n"
               "4 - Mostrar vertices, arestas e incidencias\n"
               "5 - Consultar vertice (existencia e grau)\n"
               "6 - Consultar aresta entre vertices\n"
               "7 - Remover aresta\n"
               "8 - Remover vertice\n"
               "9 - Salvar grafo em arquivo texto\n"
               "10 - Carregar grafo de arquivo texto\n"
               "11 - Exportar grafo para DOT\n"
               "0 - Voltar ao menu principal\n");
        if (!solicitaInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            int maxVertices;
            int maxArestas;
            int dirigido;
            if (!solicitaInteiro("Capacidade maxima de vertices: ",
                                 &maxVertices) ||
                !solicitaInteiro("Capacidade maxima de arestas: ",
                                 &maxArestas) ||
                !solicitaInteiro("Dirigido? (1=sim, 0=nao): ", &dirigido)) {
                break;
            }
            if (maxVertices < 0 || maxArestas < 0 ||
                (dirigido != 0 && dirigido != 1)) {
                printf("Capacidades ou tipo de grafo invalidos.\n");
                continue;
            }

            Grafo *novoGrafo =
                GGcriaGrafoTipo(maxVertices, maxArestas, dirigido);
            if (novoGrafo == NULL) {
                printf("Nao foi possivel criar o grafo.\n");
                continue;
            }
            GGdestroiGrafo(grafo);
            grafo = novoGrafo;
            printf("Grafo %s criado.\n",
                   dirigido ? "dirigido" : "nao dirigido");
        } else if (opcao == 2) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            Vertice *novoVertice = GVcriaVertice(grafo);
            if (novoVertice == NULL) {
                printf("Nao foi possivel adicionar vertice "
                       "(capacidade esgotada ou erro de memoria).\n");
            } else {
                imprimeGrafo(grafo);
            }
        } else if (opcao == 3) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            int origem;
            int destino;
            if (!solicitaInteiro("ID do vertice de origem: ", &origem) ||
                !solicitaInteiro("ID do vertice de destino: ", &destino)) {
                break;
            }
            Aresta *novaAresta = GAcriaAresta(grafo, origem, destino);
            if (novaAresta == NULL) {
                printf("Nao foi possivel adicionar aresta. Verifique os "
                       "vertices, a capacidade e se a aresta ja existe.\n");
            } else {
                imprimeGrafo(grafo);
            }
        } else if (opcao == 4) {
            imprimeGrafo(grafo);
        } else if (opcao == 5) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            int id;
            if (!solicitaInteiro("ID do vertice: ", &id)) {
                break;
            }
            if (!GBexisteIdVertice(grafo, id)) {
                printf("Vertice v%d nao encontrado.\n", id);
            } else {
                printf("Vertice v%d existe e tem grau %d.\n",
                       id, GIpegaGrau(grafo, id));
            }
        } else if (opcao == 6) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            int origem;
            int destino;
            if (!solicitaInteiro("ID do primeiro vertice: ", &origem) ||
                !solicitaInteiro("ID do segundo vertice: ", &destino)) {
                break;
            }
            int existe = GBexisteAresta(grafo, origem, destino);
            int existeNaDirecao = GBexisteArestaDir(grafo, origem, destino);
            Aresta *aresta = GApegaAresta(grafo, origem, destino);
            Aresta *arestaNaDirecao =
                GApegaArestaDir(grafo, origem, destino);
            printf("Aresta entre v%d e v%d: %s.\n", origem, destino,
                   existe ? "existe" : "nao existe");
            if (GehDirigido(grafo)) {
                printf("Aresta orientada de v%d para v%d: %s.\n",
                       origem, destino,
                       existeNaDirecao && arestaNaDirecao != NULL
                           ? "existe" : "nao existe");
            }
            printf("Consulta pela TAD: %s.\n",
                   aresta != NULL ? "aresta retornada" : "NULL retornado");
        } else if (opcao == 7) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            int id;
            if (!solicitaInteiro("ID da aresta a remover: ", &id)) {
                break;
            }
            if (GAremoveAresta(grafo, id)) {
                printf("Aresta a%d removida.\n", id);
            } else {
                printf("Aresta a%d nao encontrada.\n", id);
            }
        } else if (opcao == 8) {
            if (grafo == NULL) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            int id;
            if (!solicitaInteiro("ID do vertice a remover: ", &id)) {
                break;
            }
            if (GVremoveVertice(grafo, id)) {
                printf("Vertice v%d e suas arestas incidentes removidos.\n",
                       id);
            } else {
                printf("Vertice v%d nao encontrado.\n", id);
            }
        } else if (opcao >= 9 && opcao <= 11) {
            if (grafo == NULL && opcao != 10) {
                printf("Crie ou carregue um grafo primeiro.\n");
                continue;
            }
            char nomeArquivo[256];
            if (!solicitaTexto("Nome do arquivo: ", nomeArquivo,
                               sizeof(nomeArquivo))) {
                break;
            }

            if (opcao == 9) {
                if (GBsalvaGrafo(grafo, nomeArquivo)) {
                    printf("Grafo salvo em '%s'. O formato texto nao "
                           "preserva se o grafo e dirigido.\n", nomeArquivo);
                } else {
                    printf("Nao foi possivel salvar o grafo.\n");
                }
            } else if (opcao == 10) {
                Grafo *grafoCarregado = GGcarregaGrafo(nomeArquivo);
                if (grafoCarregado == NULL) {
                    printf("Nao foi possivel carregar o grafo.\n");
                } else {
                    GGdestroiGrafo(grafo);
                    grafo = grafoCarregado;
                    printf("Grafo carregado. O formato texto e "
                           "interpretado como nao dirigido.\n");
                    imprimeGrafo(grafo);
                }
            } else if (GBsalvaGrafoDot(grafo, nomeArquivo)) {
                printf("Grafo exportado para '%s'.\n", nomeArquivo);
            } else {
                printf("Nao foi possivel exportar o grafo.\n");
            }
        } else {
            printf("Opcao invalida.\n");
        }
    }

    GGdestroiGrafo(grafo);
}

int main(void) {
    int opcao;

    for (;;) {
        printf("\n===== Avaliacao das TADs =====\n"
               "1 - Pilha\n"
               "2 - Fila\n"
               "3 - Lista\n"
               "4 - Grafo\n"
               "0 - Encerrar\n");
        if (!solicitaInteiro("Opcao: ", &opcao)) {
            break;
        }

        if (opcao == 0) {
            break;
        } else if (opcao == 1) {
            menuPilha();
        } else if (opcao == 2) {
            menuFila();
        } else if (opcao == 3) {
            menuLista();
        } else if (opcao == 4) {
            menuGrafo();
        } else {
            printf("Opcao invalida.\n");
        }
    }

    printf("Encerrando os testes das TADs.\n");
    return 0;
}
