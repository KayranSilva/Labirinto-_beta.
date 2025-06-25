#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "config.h"

typedef struct {
    char *movimentos;  
    int aptidao;
    float pontuacao_movimentos;
    float desvio_padrao;
    int distancia;
    int colisoes;
    int passos_validos;
} Individuo;

void calcular_aptidao(Individuo *ind, Configuracoes *config);
int movimento_redundante(char anterior, char atual);
void imprimir_individuo(Individuo *ind, Configuracoes *config);
void mostrar_caminho_individuo(Individuo *ind, Configuracoes *config);

#endif