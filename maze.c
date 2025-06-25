#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **labirinto = NULL;
int inicio_x, inicio_y;
int saida_x, saida_y;

void carregar_labirinto(Configuracoes *config) {
    FILE *f = fopen(config->ARQUIVO_LABIRINTO, "r");
    if (!f) {
        printf("Erro: Nao foi possivel abrir '%s'\n", config->ARQUIVO_LABIRINTO);
        exit(1);
    }

    int linhas = 0, colunas = 0;
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) == 0) continue;
        if (colunas == 0) colunas = (int)strlen(buffer);
        linhas++;
    }

    config->LINHAS = linhas;
    config->COLUNAS = colunas;

    labirinto = malloc(linhas * sizeof(char *));
    rewind(f);
    for (int i = 0; i < linhas; i++) {
        fgets(buffer, sizeof(buffer), f);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        labirinto[i] = strdup(buffer);
    }
    fclose(f);
}

void imprimir_labirinto(Configuracoes *config) {
    for (int i = 0; i < config->LINHAS; i++) {
        printf("%s\n", labirinto[i]);
    }
}

void encontrar_posicoes(Configuracoes *config) {
    for (int i = 0; i < config->LINHAS; i++) {
        for (int j = 0; j < config->COLUNAS; j++) {
            if (labirinto[i][j] == 'S') {
                inicio_x = i;
                inicio_y = j;
            } else if (labirinto[i][j] == 'E') {
                saida_x = i;
                saida_y = j;
            }
        }
    }
}

void liberar_labirinto(Configuracoes *config) {
    for (int i = 0; i < config->LINHAS; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
}