#include "individual.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "maze.h"

typedef struct {
    char *movimentos;
    int topo;
    int capacidade;
} PilhaMovimentos;

static void init_pilha(PilhaMovimentos *p, int capacidade) {
    p->movimentos = malloc(capacidade * sizeof(char));
    p->topo = -1;
    p->capacidade = capacidade;
}

static void free_pilha(PilhaMovimentos *p) {
    free(p->movimentos);
    p->movimentos = NULL;
    p->topo = -1;
    p->capacidade = 0;
}

static void push(PilhaMovimentos *p, char mov) {
    if (p->topo < p->capacidade - 1) {
        p->movimentos[++p->topo] = mov;
    }
}

static char peek(PilhaMovimentos *p) {
    if (p->topo >= 0) {
        return p->movimentos[p->topo];
    }
    return '\0';
}

int movimento_redundante(char anterior, char atual) {
    return (anterior == 'C' && atual == 'B') ||
           (anterior == 'B' && atual == 'C') ||
           (anterior == 'E' && atual == 'D') ||
           (anterior == 'D' && atual == 'E');
}

void calcular_aptidao(Individuo *ind, Configuracoes *config) {
    PilhaMovimentos pilha;
    init_pilha(&pilha, config->MAX_MOVIMENTOS);

    int x = inicio_x, y = inicio_y;
    ind->colisoes = 0;
    ind->passos_validos = 0;
    char ultimo_mov = '\0';

    for (int i = 0; i < config->MAX_MOVIMENTOS; i++) {
        char mov = ind->movimentos[i];

        if (movimento_redundante(ultimo_mov, mov)) {
            ind->colisoes += 100;
            continue;
        }

        int nx = x, ny = y;
        switch (mov) {
            case 'C': nx--; break;
            case 'B': nx++; break;
            case 'E': ny--; break;
            case 'D': ny++; break;
        }

        if (nx < 0 || nx >= config->LINHAS || ny < 0 || ny >= config->COLUNAS ||
            labirinto[nx][ny] == '#') {
            ind->colisoes += 100;
            break;
        }

        x = nx;
        y = ny;
        ultimo_mov = mov;
        push(&pilha, mov);
        ind->passos_validos++;

        if (x == saida_x && y == saida_y) break;
    }

    ind->distancia = abs(x - saida_x) + abs(y - saida_y);


    float fator = 1.0f + config->PESO_DESVIO * ind->desvio_padrao;
    float base = 1000.0f - config->PESO_DISTANCIA * ind->distancia
                             - config->PESO_OBSTACULO * ind->colisoes;
    ind->aptidao = (base > 0.0f) ? (int)(base * fator) : 0;

    free_pilha(&pilha);
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
    char ultimo_mov = '\0';

    printf("\nVisualizando caminho:\n");
    for (int i = 0; i < ind->passos_validos; i++) {
        char m = ind->movimentos[i];
        if (movimento_redundante(ultimo_mov, m)) {
            continue;
        }

        int nx = x, ny = y;
        switch (m) {
            case 'C': nx--; break;
            case 'B': nx++; break;
            case 'E': ny--; break;
            case 'D': ny++; break;
        }

        // Só anda e marca se for válido!
        if (nx >= 0 && nx < config->LINHAS &&
            ny >= 0 && ny < config->COLUNAS &&
            temp[nx][ny] != '#') {
            x = nx;
            y = ny;
            ultimo_mov = m;
            if (temp[x][y] != 'S' && temp[x][y] != 'E') {
                temp[x][y] = (i == ind->passos_validos - 1) ? 'X' : '.';
            }
        }

        printf("\nPasso %d (%c):\n", i+1, m);
        for (int r = 0; r < config->LINHAS; r++) {
            printf(" ");
            for (int c = 0; c < config->COLUNAS; c++) {
                printf("%c ", temp[r][c]);
            }
            printf("\n");
        }

        if (x == saida_x && y == saida_y) {
            printf("\nSAIDA ENCONTRADA!\n");
            break;
        }
    }

    for (int i = 0; i < config->LINHAS; i++) {
        free(temp[i]);
    }
    free(temp);
}