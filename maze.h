#ifndef MAZE_H
#define MAZE_H

#include "config.h"

extern char **labirinto;
extern int inicio_x, inicio_y;
extern int saida_x, saida_y;

void carregar_labirinto(Configuracoes *config);
void imprimir_labirinto(Configuracoes *config);
void encontrar_posicoes(Configuracoes *config);
void liberar_labirinto(Configuracoes *config);

#endif