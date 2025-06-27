// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "config.h"
#include "maze.h"
#include "individual.h"
#include "genetic.h"

int main(int argc, char *argv[]) {
    srand((unsigned) time(NULL));

    char arquivo_config[256] = "configuracoes/config.ini";
    if (argc >= 2) {
        snprintf(arquivo_config, sizeof(arquivo_config), "configuracoes/%s", argv[1]);
    }

    Configuracoes config;
    if (!carregar_configuracao(arquivo_config, &config)) {
        printf("Erro ao carregar configuracoes!\n");
        return 1;
    }

    if (argc >= 3) {
        strncpy(config.ARQUIVO_LABIRINTO, argv[2], sizeof(config.ARQUIVO_LABIRINTO) - 1);
        config.ARQUIVO_LABIRINTO[sizeof(config.ARQUIVO_LABIRINTO) - 1] = '\0';
    }

    printf("Arquivo de configuracao: %s\n", arquivo_config);
    printf("Arquivo do labirinto : %s\n", config.ARQUIVO_LABIRINTO);

    carregar_labirinto(&config);
    encontrar_posicoes(&config);
    imprimir_labirinto(&config);

    char caminho_log[300];
    snprintf(caminho_log, sizeof(caminho_log), "logs/%s", config.ARQUIVO_LOG);

    FILE *logFile = fopen(caminho_log, "w");
    if (!logFile) {
        printf("Erro ao abrir arquivo de log.\n");
        liberar_labirinto(&config);
        return 1;
    }
    
    NoPopulacao *populacao = NULL;
    inicializar_populacao(&populacao, &config);
    if (!populacao) {
        printf("Falha ao inicializar a populacao!\n");
        fclose(logFile);
        liberar_labirinto(&config);
        return 1;
    }

    int geracao = 0;
    int solucao_encontrada = 0;

    
    while (geracao < config.MAX_GERACOES && !solucao_encontrada) {
        
        NoPopulacao *atual = populacao;
        NoPopulacao *melhor = atual;
        while (atual) {
            if (atual->individuo.aptidao > melhor->individuo.aptidao)
                melhor = atual;
            atual = atual->proximo;
        }

        
        printf("Geracao %4d - Melhor Aptidao: %6d | Dist: %2d | Colisoes: %3d | Passos: %2d | Desvio: %.2f\n",
               geracao,
               melhor->individuo.aptidao,
               melhor->individuo.distancia,
               melhor->individuo.colisoes,
               melhor->individuo.passos_validos,
               melhor->individuo.desvio_padrao);

        
        fprintf(logFile, "%d;%.1f;%s\n", geracao + 1, (float)melhor->individuo.aptidao, melhor->individuo.movimentos);
        fflush(logFile);

        if (melhor->individuo.distancia == 0) {
            printf("\nSOLUCAO ENCONTRADA na geracao %d!\n", geracao);
            printf("Caminho para a saida:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
            solucao_encontrada = 1;
            break;
        }

        if (geracao % 50 == 0) {
            printf("\nVisualizacao do melhor caminho parcial:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
        }

        nova_geracao(&populacao, &config);
        if (!populacao) {
            printf("\nA populacao foi extinta!\n");
            break;
        }

        geracao++;
    }

  
    if (!solucao_encontrada) {
        printf("\nNenhuma solucao encontrada apos %d geracoes\n", config.MAX_GERACOES);
        NoPopulacao *atual = populacao, *melhor = atual;
        while (atual) {
            if (atual->individuo.aptidao > melhor->individuo.aptidao)
                melhor = atual;
            atual = atual->proximo;
        }
        imprimir_individuo(&melhor->individuo, &config);
        mostrar_caminho_individuo(&melhor->individuo, &config);
    }


    liberar_populacao(populacao);
    liberar_labirinto(&config);
    fclose(logFile);

    return 0;
}