#ifndef GENETICO_H
#define GENETICO_H

#include "config.h"
#include "individual.h"

typedef struct NoPopulacao {
    Individuo individuo;
    struct NoPopulacao *proximo;
} NoPopulacao;

// Retorna o número de nós na lista de indivíduos
int tamanho_populacao(NoPopulacao *cabeca);

// Inicializa a população com indivíduos aleatórios, usando os parâmetros de Configuracoes
void inicializar_populacao(NoPopulacao **cabeca, Configuracoes *config);

// Seleciona um indivíduo via torneio, retornando o melhor dentre alguns escolhidos aleatoriamente
NoPopulacao* selecao_por_torneio(NoPopulacao *cabeca, Configuracoes *config);

// Realiza o cruzamento entre dois pais para gerar dois filhos
void cruzamento(Individuo *pai1, Individuo *pai2,
                Individuo *filho1, Individuo *filho2,
                Configuracoes *config);

// Aplica mutação em um indivíduo de acordo com a taxa definida nas configurações
void mutar(Individuo *ind, Configuracoes *config);

// Ordena a população em ordem decrescente da aptidão
void ordenar_populacao(NoPopulacao **cabeca);

// Gera uma nova geração aplicando elitismo, seleção, crossover e mutação
void nova_geracao(NoPopulacao **cabeca, Configuracoes *config);

// Libera a memória de todos os nós da população
void liberar_populacao(NoPopulacao *cabeca);

#endif // GENETICO_H