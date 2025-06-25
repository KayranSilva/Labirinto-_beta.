// config.h
#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int   TAM_POPULACAO;
    int   MAX_MOVIMENTOS;
    float TAXA_MUTACAO;
    float TAXA_CRUZAMENTO;
    float TAXA_ELITISMO;
    int   MAX_GERACOES;
    char  ARQUIVO_LABIRINTO[256];
    int   LINHAS;
    int   COLUNAS;
    int   PESO_DISTANCIA;
    int   PESO_OBSTACULO;
    int   PESO_DESVIO;
    int   TAM_TORNEIO;
    int   PESO_CIMA;
    int   PESO_BAIXO;
    int   PESO_ESQUERDA;
    int   PESO_DIREITA;
} Configuracoes;

// Carrega as configuracoes do arquivo .ini
int carregar_configuracao(const char *arquivo, Configuracoes *cfg);

#endif // CONFIG_H