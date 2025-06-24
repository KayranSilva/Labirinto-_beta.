#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** labirinto;
int inicio_x, inicio_y;
int saida_x, saida_y;

void carregar_labirinto(Configuracoes *config) {
    FILE* file = fopen(config->ARQUIVO_LABIRINTO, "r");
    if (!file) {
        perror("Erro ao abrir arquivo do labirinto");
        exit(1);
    }

    int linhas = 0;
    int colunas = 0;
    char linha[256];

   
    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) > 0) {
            linhas++;
            if (colunas == 0) {
                colunas = strlen(linha);
            }
        }
    }

    config->LINHAS = linhas;
    config->COLUNAS = colunas;

    rewind(file);

    
    labirinto = (char**)malloc(linhas * sizeof(char*));
    for (int i = 0; i < linhas; i++) {
        labirinto[i] = (char*)malloc((colunas + 1) * sizeof(char));
    }

  
    int i = 0;
    while (fgets(linha, sizeof(linha), file) && i < linhas) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (strlen(linha) > 0) {
            strncpy(labirinto[i], linha, colunas);
            labirinto[i][colunas] = '\0';
            i++;
        }
    }

    fclose(file);
}

void imprimir_labirinto(Configuracoes *config) {
    printf("\nLabirinto Atual (%dx%d):\n", config->LINHAS, config->COLUNAS);
    for (int i = 0; i < config->LINHAS; i++) {
        printf(" ");
        for (int j = 0; j < config->COLUNAS; j++) {
            printf("%c ", labirinto[i][j]);
        }
        printf("\n");
    }
    printf("Inicio: (%d,%d)\n", inicio_x, inicio_y);
    printf("Saida: (%d,%d)\n\n", saida_x, saida_y);
}

void encontrar_posicoes(Configuracoes *config) {
    for(int i = 0; i < config->LINHAS; i++) {
        for(int j = 0; j < config->COLUNAS; j++) {
            if(labirinto[i][j] == 'S') {
                inicio_x = i;
                inicio_y = j;
            }
            if(labirinto[i][j] == 'E') {
                saida_x = i;
                saida_y = j;
            }
        }
    }
}

void liberar_labirinto(Configuracoes *config) {
    for(int i = 0; i < config->LINHAS; i++) {
        free(labirinto[i]);
    }
    free(labirinto);
}