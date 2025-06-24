#ifndef GENETICO_H
#define GENETICO_H

#include "individual.h"
#include "config.h"

typedef struct NoPopulacao {
    Individuo individuo;
    struct NoPopulacao *proximo;
} NoPopulacao;

void inicializar_populacao(NoPopulacao **cabeca, Configuracoes *config);
NoPopulacao* selecao_por_torneio(NoPopulacao *cabeca, Configuracoes *config);
void cruzamento(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2, Configuracoes *config);
void mutar(Individuo *ind, Configuracoes *config);
void nova_geracao(NoPopulacao **cabeca, Configuracoes *config);
void liberar_populacao(NoPopulacao *cabeca);
int tamanho_populacao(NoPopulacao *cabeca);
void ordenar_populacao(NoPopulacao **cabeca);

#endif