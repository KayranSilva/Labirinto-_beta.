#ifndef GENETIC_H
#define GENETIC_H

#include "config.h"
#include "individual.h"

typedef struct NoPopulacao {
    Individuo individuo;
    struct NoPopulacao *proximo;
} NoPopulacao;

int tamanho_populacao(NoPopulacao *cabeca);
void inicializar_populacao(NoPopulacao **cabeca, Configuracoes *config);
NoPopulacao* selecao_por_torneio(NoPopulacao *cabeca, Configuracoes *config);
void cruzamento(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2, Configuracoes *config);
void mutar(Individuo *ind, Configuracoes *config);
void ordenar_populacao(NoPopulacao **cabeca);
void nova_geracao(NoPopulacao **cabeca, Configuracoes *config);
void liberar_populacao(NoPopulacao *cabeca);

#endif