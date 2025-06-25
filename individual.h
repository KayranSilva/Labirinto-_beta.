// individual.h
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "config.h"
#include "maze.h"

// Estrutura que representa um individuo (solucao candidata)
typedef struct {
    char   *movimentos;       // sequencia de movimentos (C, B, E, D)
    int    aptidao;              // valor da funcao fitness
    float  pontuacao_movimentos; // soma ponderada dos movimentos validos
    float  desvio_padrao;        // desvio padrao da pontuacao dos movimentos
    int    distancia;            // distancia heuristica ate a saida
    int    colisoes;             // penalidade por colisoes
    int    passos_validos;       // quantidade de movimentos validos executados
} Individuo;

void calcular_aptidao(Individuo *ind, Configuracoes *config);
void calcular_metricas_movimentos(Individuo *ind, Configuracoes *config);
int  movimento_redundante(char anterior, char atual);
void imprimir_individuo(Individuo *ind, Configuracoes *config);
void mostrar_caminho_individuo(Individuo *ind, Configuracoes *config);

#endif // INDIVIDUAL_H