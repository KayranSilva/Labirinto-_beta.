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

    // Caminho padrão para o arquivo de configuração na pasta "configuracoes"
    char arquivo_config[256] = "configuracoes/config.ini";
    if (argc >= 2) {
        // Se o usuário passar um nome, busca na pasta "configuracoes"
        snprintf(arquivo_config, sizeof(arquivo_config), "configuracoes/%s", argv[1]);
    }

    // Carrega as configurações do arquivo .ini
    Configuracoes config;
    if (!carregar_configuracao(arquivo_config, &config)) {
        printf("Erro ao carregar configuracoes!\n");
        return 1;
    }

    // Se for passado um segundo argumento, usa-o como arquivo do labirinto
    if (argc >= 3) {
        strncpy(config.ARQUIVO_LABIRINTO, argv[2], sizeof(config.ARQUIVO_LABIRINTO) - 1);
        config.ARQUIVO_LABIRINTO[sizeof(config.ARQUIVO_LABIRINTO) - 1] = '\0';
    }

    printf("Arquivo de configuracao: %s\n", arquivo_config);
    printf("Arquivo do labirinto : %s\n", config.ARQUIVO_LABIRINTO);

    // Carrega o labirinto, encontra as posições de início e saída, e exibe o labirinto
    carregar_labirinto(&config);
    encontrar_posicoes(&config);
    imprimir_labirinto(&config);

    // Monta o caminho completo para o log na pasta logs/
    char caminho_log[300];
    snprintf(caminho_log, sizeof(caminho_log), "logs/%s", config.ARQUIVO_LOG);

    // Abre (ou recria) o arquivo CSV de log, sobrescrevendo o conteúdo anterior
    FILE *logFile = fopen(caminho_log, "w");
    if (!logFile) {
        printf("Erro ao abrir arquivo de log.\n");
        liberar_labirinto(&config);
        return 1;
    }
    // Opcional: escreva um cabeçalho no CSV (caso deseje)
    // fprintf(logFile, "Geracao;Fitness;Movimentos\n");

    // Inicializa a população
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

    // Loop principal das gerações do algoritmo genético
    while (geracao < config.MAX_GERACOES && !solucao_encontrada) {
        // Encontra o melhor indivíduo na população
        NoPopulacao *atual = populacao;
        NoPopulacao *melhor = atual;
        while (atual) {
            if (atual->individuo.aptidao > melhor->individuo.aptidao)
                melhor = atual;
            atual = atual->proximo;
        }

        // Exibe estatísticas da geração atual
        printf("Geracao %4d - Melhor Aptidao: %6d | Dist: %2d | Colisoes: %3d | Passos: %2d | Desvio: %.2f\n",
               geracao,
               melhor->individuo.aptidao,
               melhor->individuo.distancia,
               melhor->individuo.colisoes,
               melhor->individuo.passos_validos,
               melhor->individuo.desvio_padrao);

        // Registra no CSV: ID da geração (geracao+1), fitness (com uma casa decimal) e a representação textual dos movimentos
        // Utilizamos ponto e vírgula (";") como delimitador para maior compatibilidade com configurações regionais.
        fprintf(logFile, "%d;%.1f;%s\n", geracao + 1, (float)melhor->individuo.aptidao, melhor->individuo.movimentos);
        fflush(logFile);

        // Se a solução foi encontrada (distância = 0), exibe o caminho e encerra o loop
        if (melhor->individuo.distancia == 0) {
            printf("\nSOLUCAO ENCONTRADA na geracao %d!\n", geracao);
            printf("Caminho para a saida:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
            solucao_encontrada = 1;
            break;
        }

        // Exibe periodicamente o caminho parcial do melhor indivíduo (a cada 50 gerações)
        if (geracao % 50 == 0) {
            printf("\nVisualizacao do melhor caminho parcial:\n");
            mostrar_caminho_individuo(&melhor->individuo, &config);
        }

        // Gera a nova geração (aplicando elitismo, seleção, cruzamento e mutação)
        nova_geracao(&populacao, &config);
        if (!populacao) {
            printf("\nA populacao foi extinta!\n");
            break;
        }

        geracao++;
    }

    // Se nenhuma solução foi encontrada após o número máximo de gerações,
    // exibe o melhor indivíduo obtido.
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

    // Libera os recursos e fecha o arquivo de log
    liberar_populacao(populacao);
    liberar_labirinto(&config);
    fclose(logFile);

    return 0;
}