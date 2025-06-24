#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

int carregar_configuracao(const char* arquivo, Configuracoes *config) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return 0;

    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        char chave[64], valor[256];
        if (sscanf(linha, "%63[^=]=%255s", chave, valor) == 2) {
            if (strcmp(chave, "TAM_POPULACAO") == 0) config->TAM_POPULACAO = atoi(valor);
            else if (strcmp(chave, "MAX_MOVIMENTOS") == 0) config->MAX_MOVIMENTOS = atoi(valor);
            else if (strcmp(chave, "TAXA_MUTACAO") == 0) config->TAXA_MUTACAO = (float)atof(valor);
            else if (strcmp(chave, "TAXA_CRUZAMENTO") == 0) config->TAXA_CRUZAMENTO = (float)atof(valor);
            else if (strcmp(chave, "TAXA_ELITISMO") == 0) config->TAXA_ELITISMO = (float)atof(valor);
            else if (strcmp(chave, "MAX_GERACOES") == 0) config->MAX_GERACOES = atoi(valor);
            else if (strcmp(chave, "ARQUIVO_LABIRINTO") == 0) strncpy(config->ARQUIVO_LABIRINTO, valor, 255);
            else if (strcmp(chave, "LINHAS") == 0) config->LINHAS = atoi(valor);
            else if (strcmp(chave, "COLUNAS") == 0) config->COLUNAS = atoi(valor);
            else if (strcmp(chave, "PESO_DISTANCIA") == 0) config->PESO_DISTANCIA = atoi(valor);
            else if (strcmp(chave, "PESO_OBSTACULO") == 0) config->PESO_OBSTACULO = atoi(valor);
            else if (strcmp(chave, "PESO_DESVIO") == 0) config->PESO_DESVIO = atoi(valor);
            else if (strcmp(chave, "TAM_TORNEIO") == 0) config->TAM_TORNEIO = atoi(valor);
            else if (strcmp(chave, "PESO_CIMA") == 0) config->PESO_CIMA = atoi(valor);
            else if (strcmp(chave, "PESO_BAIXO") == 0) config->PESO_BAIXO = atoi(valor);
            else if (strcmp(chave, "PESO_ESQUERDA") == 0) config->PESO_ESQUERDA = atoi(valor);
            else if (strcmp(chave, "PESO_DIREITA") == 0) config->PESO_DIREITA = atoi(valor);
        }
    }
    fclose(f);
    return 1;
}