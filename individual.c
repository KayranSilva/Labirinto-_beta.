#include "individual.h"
#include "genetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int movimento_redundante(char anterior, char atual) {
    if (anterior == '\0') return 0;
    return (anterior == 'C' && atual == 'B') ||
           (anterior == 'B' && atual == 'C') ||
           (anterior == 'E' && atual == 'D') ||
           (anterior == 'D' && atual == 'E');
}

void calcular_metricas_movimentos(Individuo *ind, Configuracoes *config) {
    ind->pontuacao_movimentos = 0.0f;
    float soma2 = 0.0f;
    int cont[4] = {0};

    for (int i = 0; i < ind->passos_validos; i++) {
        char m = ind->movimentos[i];
        switch (m) {
            case 'C': 
                ind->pontuacao_movimentos += config->PESO_CIMA;
                cont[0]++; 
                break;
            case 'B': 
                ind->pontuacao_movimentos += config->PESO_BAIXO;
                cont[1]++; 
                break;
            case 'E': 
                ind->pontuacao_movimentos += config->PESO_ESQUERDA;
                cont[2]++; 
                break;
            case 'D': 
                ind->pontuacao_movimentos += config->PESO_DIREITA;
                cont[3]++; 
                break;
        }
    }

    if (ind->passos_validos > 0) {
        float media = ind->pontuacao_movimentos / ind->passos_validos;
        for (int i = 0; i < 4; i++) {
            float peso = (i==0) ? config->PESO_CIMA :
                        (i==1) ? config->PESO_BAIXO :
                        (i==2) ? config->PESO_ESQUERDA :
                        config->PESO_DIREITA;
            soma2 += (peso - media) * (peso - media) * cont[i];
        }
        ind->desvio_padrao = sqrtf(soma2 / ind->passos_validos);
    } else {
        ind->desvio_padrao = 0.0f;
    }
}

void calcular_aptidao(Individuo *ind, Configuracoes *config) {
    int x = inicio_x, y = inicio_y;
    ind->colisoes = 0;
    ind->passos_validos = 0;
    char ult = '\0';

    for (int i = 0; i < config->MAX_MOVIMENTOS; i++) {
        char m = ind->movimentos[i];
        
        if (movimento_redundante(ult, m)) {
            ind->colisoes += 100;
            continue;
        }

        int nx = x, ny = y;
        switch (m) {
            case 'C': nx--; break;
            case 'B': nx++; break;
            case 'E': ny--; break;
            case 'D': ny++; break;
        }

        if (nx < 0 || nx >= config->LINHAS ||
            ny < 0 || ny >= config->COLUNAS ||
            labirinto[nx][ny] == '#') {
            ind->colisoes += 100;
            break;
        }

        x = nx; y = ny;
        ult = m;
        ind->passos_validos++;
        
        if (x == saida_x && y == saida_y) break;
    }

    ind->distancia = abs(x - saida_x) + abs(y - saida_y);
    calcular_metricas_movimentos(ind, config);

    float fator = 1.0f + config->PESO_DESVIO * ind->desvio_padrao;
    float base = 1000.0f - config->PESO_DISTANCIA * ind->distancia
                             - config->PESO_OBSTACULO * ind->colisoes;
    ind->aptidao = (base > 0.0f) ? (int)(base * fator) : 0;
}

void imprimir_individuo(Individuo *ind, Configuracoes *config) {
    printf("Aptidao: %d | Distancia: %d | Colisoes: %d | Passos: %d\n",
           ind->aptidao, ind->distancia, ind->colisoes, ind->passos_validos);
    printf("Movimentos: ");
    for (int i = 0; i < ind->passos_validos; i++) {
        printf("%c ", ind->movimentos[i]);
    }
    printf("\n");
}

void mostrar_caminho_individuo(Individuo *ind, Configuracoes *config) {
    char **temp = malloc(config->LINHAS * sizeof(char *));
    for (int i = 0; i < config->LINHAS; i++) {
        temp[i] = malloc(config->COLUNAS + 1);
        strncpy(temp[i], labirinto[i], config->COLUNAS);
        temp[i][config->COLUNAS] = '\0';
    }

    int x = inicio_x, y = inicio_y;
    temp[x][y] = 'S';
    printf("\nVisualizando caminho:\n");

    char ult = '\0';
    for (int i = 0; i < ind->passos_validos; i++) {
        char m = ind->movimentos[i];
        if (movimento_redundante(ult, m)) {
            ult = m;
            continue;
        }

        switch (m) {
            case 'C': x--; break;
            case 'B': x++; break;
            case 'E': y--; break;
            case 'D': y++; break;
        }
        ult = m;

        if (temp[x][y] != 'S' && temp[x][y] != 'E') {
            temp[x][y] = (i == ind->passos_validos - 1) ? 'X' : '.';
        }

        printf("\nPasso %d (%c):\n", i+1, m);
        for (int r = 0; r < config->LINHAS; r++) {
            printf(" ");
            for (int c = 0; c < config->COLUNAS; c++) {
                printf("%c ", temp[r][c]);
            }
            printf("\n");
        }
    }
}