#ifndef MAZE_H
#define MAZE_H

#include "config.h"

// Ponteiros e variáveis globais para o labirinto
extern char **labirinto;
extern int inicio_x;
extern int inicio_y;
extern int saida_x;
extern int saida_y;

// Carrega o labirinto a partir do arquivo indicado em ARQUIVO_LABIRINTO (arquivo .ini)
void carregar_labirinto(Configuracoes *config);

// Imprime o labirinto na tela
void imprimir_labirinto(Configuracoes *config);

// Encontra as posicoes de inicio ('S') e de saida ('E') no labirinto
void encontrar_posicoes(Configuracoes *config);

// Libera a memoria alocada para o labirinto
void liberar_labirinto(Configuracoes *config);

#endif // MAZE_H