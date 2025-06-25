#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **labirinto = NULL;
int inicio_x, inicio_y;
int saida_x, saida_y;

void carregar_labirinto(Configuracoes *config) {
    printf("Carregando labirinto: %s\n", config->ARQUIVO_LABIRINTO);
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
        else if ((int)strlen(buffer) != colunas) {
            printf("Erro: Labirinto deve ter linhas de mesmo tamanho\n");
            fclose(f);
            exit(1);
        }
        linhas++;
    }

    if (linhas == 0 || colunas == 0) {
        printf("Erro: Labirinto vazio ou invalido\n");
        fclose(f);
        exit(1);
    }

    config->LINHAS = linhas;
    config->COLUNAS = colunas;

    labirinto = malloc(linhas * sizeof(char *));
    if (!labirinto) {
        printf("Erro: Falha ao alocar memoria para labirinto\n");
        fclose(f);
        exit(1);
    }

    rewind(f);
    for (int i = 0; i < linhas; i++) {
        if (!fgets(buffer, sizeof(buffer), f)) break;
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) != colunas) continue;

        labirinto[i] = malloc(colunas + 1);
        if (!labirinto[i]) {
            printf("Erro: Falha ao alocar linha %d\n", i);
            for (int j = 0; j < i; j++) free(labirinto[j]);
            free(labirinto);
            fclose(f);
            exit(1);
        }
        strcpy(labirinto[i], buffer);
    }
    fclose(f);

}

void imprimir_labirinto(Configuracoes *config) {
    printf("\nLabirinto (%dx%d):\n", config->LINHAS, config->COLUNAS);
    for (int i = 0; i < config->LINHAS; i++) {
        printf(" ");
        for (int j = 0; j < config->COLUNAS; j++) {
            printf("%c ", labirinto[i][j]);
        }
        printf("\n");
    }
    printf("Inicio: (%d,%d)\n", inicio_x, inicio_y);
    printf("Saida:  (%d,%d)\n\n", saida_x, saida_y);
}

void encontrar_posicoes(Configuracoes *config) {
    inicio_x = inicio_y = -1;
    saida_x = saida_y = -1;

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

    if (inicio_x == -1 || inicio_y == -1) {
        printf("Erro: Posicao inicial 'S' nao encontrada\n");
        exit(1);
    }
    if (saida_x == -1 || saida_y == -1) {
        printf("Erro: Posicao final 'E' nao encontrada\n");
        exit(1);
    }
}

void liberar_labirinto(Configuracoes *config) {
    if (labirinto) {
        for (int i = 0; i < config->LINHAS; i++) {
            free(labirinto[i]);
        }
        free(labirinto);
        labirinto = NULL;
    }
}