// config.c
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* ltrim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

static void rtrim(char *s) {
    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}

static char* trim(char *s) {
    char *start = ltrim(s);
    rtrim(start);
    return start;
}

static void default_config(Configuracoes *cfg) {
    cfg->TAM_POPULACAO   = 300;
    cfg->MAX_MOVIMENTOS  = 60;
    cfg->TAXA_MUTACAO    = 0.20f;
    cfg->TAXA_CRUZAMENTO = 0.95f;
    cfg->TAXA_ELITISMO   = 0.50f;
    cfg->MAX_GERACOES    = 500;
    strcpy(cfg->ARQUIVO_LABIRINTO, "maze2.txt");
    cfg->LINHAS          = 0;
    cfg->COLUNAS         = 0;
    cfg->PESO_DISTANCIA  = 100;
    cfg->PESO_OBSTACULO  = 100;
    cfg->PESO_DESVIO     = 1;
    cfg->TAM_TORNEIO     = 5;
    cfg->PESO_CIMA       = 1;
    cfg->PESO_BAIXO      = 2;
    cfg->PESO_ESQUERDA   = 3;
    cfg->PESO_DIREITA    = 4;
}

int carregar_configuracao(const char *arquivo, Configuracoes *cfg) {
    FILE *f = fopen(arquivo, "r");
    if (!f) {
        printf("Erro: Nao foi possivel abrir '%s'\n", arquivo);
        return 0;
    }

    default_config(cfg);

    char linha[512];
    while (fgets(linha, sizeof linha, f)) {
        linha[strcspn(linha, "\r\n")] = '\0';
        char *p = trim(linha);
        if (*p == '\0' || *p == '#' || *p == ';') continue;

        char *coment = strpbrk(p, "#;");
        if (coment) *coment = '\0';

        char *eq = strchr(p, '=');
        if (!eq) continue;
        *eq = '\0';
        char *key = trim(p);
        char *value = trim(eq + 1);

        if (strcmp(key, "TAM_POPULACAO") == 0) {
            cfg->TAM_POPULACAO = atoi(value);
        }
        else if (strcmp(key, "MAX_MOVIMENTOS") == 0) cfg->MAX_MOVIMENTOS = atoi(value);
        else if (strcmp(key, "TAXA_MUTACAO") == 0) cfg->TAXA_MUTACAO = atof(value);
        else if (strcmp(key, "TAXA_CRUZAMENTO") == 0) cfg->TAXA_CRUZAMENTO = atof(value);
        else if (strcmp(key, "TAXA_ELITISMO") == 0) cfg->TAXA_ELITISMO = atof(value);
        else if (strcmp(key, "MAX_GERACOES") == 0) cfg->MAX_GERACOES = atoi(value);
        else if (strcmp(key, "ARQUIVO_LABIRINTO") == 0) strcpy(cfg->ARQUIVO_LABIRINTO, value);
        else if (strcmp(key, "LINHAS") == 0) cfg->LINHAS = atoi(value);
        else if (strcmp(key, "COLUNAS") == 0) cfg->COLUNAS = atoi(value);
        else if (strcmp(key, "PESO_DISTANCIA") == 0) cfg->PESO_DISTANCIA = atoi(value);
        else if (strcmp(key, "PESO_OBSTACULO") == 0) cfg->PESO_OBSTACULO = atoi(value);
        else if (strcmp(key, "PESO_DESVIO") == 0) cfg->PESO_DESVIO = atoi(value);
        else if (strcmp(key, "TAM_TORNEIO") == 0) cfg->TAM_TORNEIO = atoi(value);
        else if (strcmp(key, "PESO_CIMA") == 0) cfg->PESO_CIMA = atoi(value);
        else if (strcmp(key, "PESO_BAIXO") == 0) cfg->PESO_BAIXO = atoi(value);
        else if (strcmp(key, "PESO_ESQUERDA") == 0) cfg->PESO_ESQUERDA = atoi(value);
        else if (strcmp(key, "PESO_DIREITA") == 0) cfg->PESO_DIREITA = atoi(value);
    }

    fclose(f);
    return 1;
}