#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "config.h"
#include "maze.h"
#include "individual.h"
#include "genetic.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

   
    char arquivo_config[256] = "config.ini";

    if (argc >= 2) {
        strncpy(arquivo_config, argv[1], sizeof(arquivo_config) - 1);
        arquivo_config[sizeof(arquivo_config) - 1] = '\0';
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
    printf("Arquivo do labirinto: %s\n", config.ARQUIVO_LABIRINTO);

  

    carregar_labirinto(&config);
    encontrar_posicoes(&config);
    imprimir_labirinto(&config);

    
    NoPopulacao *populacao = NULL;
    inicializar_populacao(&populacao, &config);

    if(populacao == NULL) {
        printf("Falha ao inicializar a populacao!\n");
        liberar_labirinto(&config);
        return 1;
    }

    int geracao = 0;
    int solucao_encontrada = 0;

    printf("\nPopulacao inicial:\n");
    NoPopulacao *atual = populacao;
    while(geracao < config.MAX_GERACOES && !solucao_encontrada) {
    
        NoPopulacao *atual = populacao;
        NoPopulacao *melhor = atual;

        while(atual != NULL) {
            if(atual->individuo.aptidao > melhor->individuo.aptidao) {
                melhor = atual;
            }
            atual = atual->proximo;
        }

        printf("Geracao %4d - ", geracao);
        printf("Melhor: %6d | ", melhor->individuo.aptidao);
        printf("Dist: %2d | ", melhor->individuo.distancia);
        printf("Col: %3d | ", melhor->individuo.colisoes);
        printf("Passos: %2d | ", melhor->individuo.passos_validos);
        printf("Desvio: %.2f\n", melhor->individuo.desvio_padrao);


        if(melhor->individuo.distancia == 0) {
            printf("\nSOLUCAO ENCONTRADA na geracao %d!\n", geracao);
            printf("Caminho para a saida:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
            solucao_encontrada = 1;
            break;
        }

        if(geracao % 50 == 0) {
            printf("\nVisualizacao do melhor caminho parcial:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
        }


        nova_geracao(&populacao, &config);

        if(populacao == NULL) {
            printf("\nA populacao foi extinta!\n");
            break;
        }

        geracao++;
    }

   if(!solucao_encontrada) {
        printf("\nNenhuma solucao encontrada apos %d geracoes\n", config.MAX_GERACOES);
        printf("Mostrando a melhor solucao encontrada:\n");
        NoPopulacao *atual = populacao;
        NoPopulacao *melhor = atual;
        while(atual != NULL) {
            if(atual->individuo.aptidao > melhor->individuo.aptidao) {
                melhor = atual;
            }
            atual = atual->proximo;
        }
        #ifdef _WIN32
            system("pause");
        #endif

        printf("\nMelhor solucao encontrada:\n");
        imprimir_individuo(&melhor->individuo, &config);
        mostrar_caminho_individuo(&melhor->individuo, &config);
        #ifdef _WIN32
            system("pause");
        #endif
    }


    liberar_populacao(populacao);
    liberar_labirinto(&config);

    #ifdef _WIN32
        system("pause");
    #endif

    return 0;
}