#include "individual.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int movimento_redundante(char anterior, char atual) {
    return (anterior == 'C' && atual == 'B') || 
           (anterior == 'B' && atual == 'C') ||
           (anterior == 'E' && atual == 'D') || 
           (anterior == 'D' && atual == 'E');
}

void calcular_metricas_movimentos(Individuo *ind, Configuracoes *config) {
    ind->pontuacao_movimentos = 0;
    float soma_quadrados = 0;
    int contagem_movimentos[4] = {0};
    
    for(int i = 0; i < ind->passos_validos; i++) {
        switch(ind->movimentos[i]) {
            case 'C': 
                ind->pontuacao_movimentos += config->PESO_CIMA;
                contagem_movimentos[0]++;
                break;
            case 'B':
                ind->pontuacao_movimentos += config->PESO_BAIXO;
                contagem_movimentos[1]++;
                break;
            case 'E':
                ind->pontuacao_movimentos += config->PESO_ESQUERDA;
                contagem_movimentos[2]++;
                break;
            case 'D':
                ind->pontuacao_movimentos += config->PESO_DIREITA;
                contagem_movimentos[3]++;
                break;
        }
    }
    
    if(ind->passos_validos > 0) {
        float media = ind->pontuacao_movimentos / ind->passos_validos;
        for(int i = 0; i < 4; i++) {
            float valor;
            switch(i) {
                case 0: valor = config->PESO_CIMA; break;
                case 1: valor = config->PESO_BAIXO; break;
                case 2: valor = config->PESO_ESQUERDA; break;
                case 3: valor = config->PESO_DIREITA; break;
            }
            soma_quadrados += (valor - media) * (valor - media) * contagem_movimentos[i];
        }
        ind->desvio_padrao = sqrt(soma_quadrados / ind->passos_validos);
    } else {
        ind->desvio_padrao = 0;
    }
}

void calcular_aptidao(Individuo *ind, Configuracoes *config) {
    int x = inicio_x, y = inicio_y;
    ind->colisoes = 0;
    ind->passos_validos = 0;
    char ultimo_movimento = '\0';
    
    for(int i = 0; i < config->MAX_MOVIMENTOS; i++) {
        if(movimento_redundante(ultimo_movimento, ind->movimentos[i])) {
            ind->colisoes += 100;
            continue;
        }

        int novo_x = x, novo_y = y;
        switch(ind->movimentos[i]) {
            case 'C': novo_x--; break;
            case 'B': novo_x++; break;
            case 'E': novo_y--; break;
            case 'D': novo_y++; break;
        }

        if(novo_x < 0 || novo_x >= config->LINHAS || novo_y < 0 || novo_y >= config->COLUNAS || 
           labirinto[novo_x][novo_y] == '#') {
            ind->colisoes += 100;
            break;
        }

        x = novo_x;
        y = novo_y;
        ultimo_movimento = ind->movimentos[i];
        ind->passos_validos++;
        
        if(x == saida_x && y == saida_y) break;
    }

    ind->distancia = abs(x - saida_x) + abs(y - saida_y);
    calcular_metricas_movimentos(ind, config);
    
    float fator_desvio = (1 + config->PESO_DESVIO * ind->desvio_padrao);
    float aptidao_base = 1000 - (config->PESO_DISTANCIA * ind->distancia) - (config->PESO_OBSTACULO * ind->colisoes);
    
    ind->aptidao = aptidao_base > 0 ? aptidao_base * fator_desvio : 0;
}

void mostrar_caminho_individuo(Individuo *ind, Configuracoes *config) {
    char** labirinto_temp = (char**)malloc(config->LINHAS * sizeof(char*));
    for(int i = 0; i < config->LINHAS; i++) {
        labirinto_temp[i] = (char*)malloc((config->COLUNAS + 1) * sizeof(char));
        strncpy(labirinto_temp[i], labirinto[i], config->COLUNAS);
        labirinto_temp[i][config->COLUNAS] = '\0';
    }

    int x = inicio_x, y = inicio_y;
    labirinto_temp[x][y] = 'S';
    char ultimo_movimento = '\0';

    printf("\nVisualizando caminho:\n");
    for (int i = 0; i < ind->passos_validos; i++) {
        if(movimento_redundante(ultimo_movimento, ind->movimentos[i])) {
            continue;
        }

        int novo_x = x, novo_y = y;
        switch(ind->movimentos[i]) {
            case 'C': novo_x--; break;
            case 'B': novo_x++; break;
            case 'E': novo_y--; break;
            case 'D': novo_y++; break;
        }

        x = novo_x;
        y = novo_y;
        ultimo_movimento = ind->movimentos[i];

        if (labirinto_temp[x][y] != 'S' && labirinto_temp[x][y] != 'E') {
            labirinto_temp[x][y] = '.';
        }

        printf("\nPasso %d (%c):\n", i+1, ind->movimentos[i]);
        for (int r = 0; r < config->LINHAS; r++) {
            printf(" ");
            for (int c = 0; c < config->COLUNAS; c++) {
                printf("%c ", labirinto_temp[r][c]);
            }
            printf("\n");
        }

        if (x == saida_x && y == saida_y) {
            printf("\nSAIDA ENCONTRADA!\n");
            break;
        }

        #ifdef _WIN32
            Sleep(300);
        #else
            usleep(300000);
        #endif
    }

    for(int i = 0; i < config->LINHAS; i++) {
        free(labirinto_temp[i]);
    }
    free(labirinto_temp);
}

void imprimir_individuo(Individuo *ind, Configuracoes *config) {
    
    
    printf("Aptidao: %d | Distancia: %d | Colisoes: %d | Passos: %d\n",
           ind->aptidao, ind->distancia, ind->colisoes, ind->passos_validos);
    printf("Movimentos: ");
    for(int i = 0; i < ind->passos_validos; i++) {
        printf("%c ", ind->movimentos[i]);
    }
    printf("\n");
}