#include "genetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int tamanho_populacao(NoPopulacao *cabeca) {
    int tamanho = 0;
    while (cabeca) {
        tamanho++;
        cabeca = cabeca->proximo;
    }
    return tamanho;
}

void inicializar_populacao(NoPopulacao **cabeca, Configuracoes *config) {
    *cabeca = NULL;
    NoPopulacao *cauda = NULL;
    srand((unsigned)time(NULL));

    for (int i = 0; i < config->TAM_POPULACAO; i++) {
        NoPopulacao *novo_no = malloc(sizeof(NoPopulacao));
        if (!novo_no) {
            printf("Erro: Falha ao alocar novo individuo\n");
            liberar_populacao(*cabeca);
            exit(1);
        }
        novo_no->individuo.movimentos = malloc(config->MAX_MOVIMENTOS * sizeof(char));
        if (!novo_no->individuo.movimentos) {
            printf("Erro: Falha ao alocar movimentos\n");
            free(novo_no);
            liberar_populacao(*cabeca);
            exit(1);
        }

        char ultimo_mov = '\0';
        for (int j = 0; j < config->MAX_MOVIMENTOS; j++) {
            char mov;
            int tent = 0;
            do {
                int r = rand() % 4;
                char ops[4] = { 'C', 'B', 'E', 'D' };
                mov = ops[r];
                tent++;
                if (!movimento_redundante(ultimo_mov, mov) || tent > 2)
                    break;
            } while (1);
            novo_no->individuo.movimentos[j] = mov;
            ultimo_mov = mov;
        }

        calcular_aptidao(&novo_no->individuo, config);
        novo_no->proximo = NULL;

        if (!*cabeca) {
            *cabeca = novo_no;
            cauda = novo_no;
        } else {
            cauda->proximo = novo_no;
            cauda = novo_no;
        }
    }
}

NoPopulacao* selecao_por_torneio(NoPopulacao *cabeca, Configuracoes *config) {
    if (!cabeca) return NULL;

    int tam = tamanho_populacao(cabeca);
    NoPopulacao *melhor = NULL;
    float best_fit = -1e9f;

    for (int i = 0; i < config->TAM_TORNEIO; i++) {
        int idx = rand() % tam;
        NoPopulacao *atual = cabeca;
        for (int j = 0; j < idx && atual; j++) {
            atual = atual->proximo;
        }
        if (atual && (!melhor || atual->individuo.aptidao > best_fit)) {
            melhor = atual;
            best_fit = atual->individuo.aptidao;
        }
    }
    return melhor;
}

void cruzamento(Individuo *p1, Individuo *p2,
                Individuo *f1, Individuo *f2,
                Configuracoes *config) {
    if (!f1 || !f2) {
        printf("Erro: Ponteiro de filho nulo em cruzamento\n");
        exit(1);
    }
    f1->movimentos = (char*)malloc(config->MAX_MOVIMENTOS * sizeof(char));
    f2->movimentos = (char*)malloc(config->MAX_MOVIMENTOS * sizeof(char));
    if (!f1->movimentos || !f2->movimentos) {
        printf("Erro: Falha ao alocar movimentos para filhos\n");
        exit(1);
    }

    if ((double)rand() / RAND_MAX > config->TAXA_CRUZAMENTO) {
        memcpy(f1->movimentos, p1->movimentos, config->MAX_MOVIMENTOS);
        memcpy(f2->movimentos, p2->movimentos, config->MAX_MOVIMENTOS);
        return;
    }

    int m = config->MAX_MOVIMENTOS;
    int pt1 = rand() % m;
    int pt2 = rand() % m;
    if (pt1 > pt2) {
        int t = pt1; pt1 = pt2; pt2 = t;
    }

    for (int i = 0; i < pt1; i++) {
        f1->movimentos[i] = p1->movimentos[i];
        f2->movimentos[i] = p2->movimentos[i];
    }
    for (int i = pt1; i < pt2; i++) {
        f1->movimentos[i] = p2->movimentos[i];
        f2->movimentos[i] = p1->movimentos[i];
    }
    for (int i = pt2; i < m; i++) {
        f1->movimentos[i] = p1->movimentos[i];
        f2->movimentos[i] = p2->movimentos[i];
    }
}

void mutar(Individuo *ind, Configuracoes *config) {
    char ultimo_mov = '\0';
    int m = config->MAX_MOVIMENTOS;

    for (int i = 0; i < m; i++) {
        if ((double)rand() / RAND_MAX < config->TAXA_MUTACAO) {
            char novo;
            int tent = 0;
            do {
                int r = rand() % 4;
                char ops[4] = { 'C', 'B', 'E', 'D' };
                novo = ops[r];
                tent++;
                if (!movimento_redundante(ultimo_mov, novo) || tent > 2)
                    break;
            } while (1);
            ind->movimentos[i] = novo;
        }
        ultimo_mov = ind->movimentos[i];
    }
}

void ordenar_populacao(NoPopulacao **cabeca) {
    if (!cabeca || !*cabeca || !(*cabeca)->proximo) return;

    NoPopulacao *ord = NULL;
    NoPopulacao *atual = *cabeca;
    while (atual) {
        NoPopulacao *prox = atual->proximo;
        if (!ord || atual->individuo.aptidao > ord->individuo.aptidao) {
            atual->proximo = ord;
            ord = atual;
        } else {
            NoPopulacao *tmp = ord;
            while (tmp->proximo &&
                   atual->individuo.aptidao <= tmp->proximo->individuo.aptidao) {
                tmp = tmp->proximo;
            }
            atual->proximo = tmp->proximo;
            tmp->proximo = atual;
        }
        atual = prox;
    }
    *cabeca = ord;
}

void nova_geracao(NoPopulacao **cabeca, Configuracoes *config) {
    if (!cabeca || !*cabeca) return;

    ordenar_populacao(cabeca);

    int pop_size = config->TAM_POPULACAO;
    int elite_cnt = (int)(pop_size * config->TAXA_ELITISMO);
    if (elite_cnt < 1) elite_cnt = 1;

    NoPopulacao *old = *cabeca;
    NoPopulacao *nova = NULL;
    NoPopulacao *cauda = NULL;

    // Copia elite
    for (int i = 0; i < elite_cnt && old; i++) {
        NoPopulacao *n = malloc(sizeof(NoPopulacao));
        if (!n) {
            printf("Erro: Falha ao alocar elite\n");
            liberar_populacao(nova);
            exit(1);
        }
        n->individuo.movimentos = malloc(config->MAX_MOVIMENTOS * sizeof(char));
        if (!n->individuo.movimentos) {
            printf("Erro: Falha ao alocar movimentos para elite\n");
            free(n);
            liberar_populacao(nova);
            exit(1);
        }
        // Copia todos os campos exceto ponteiro movimentos
        memcpy(n->individuo.movimentos, old->individuo.movimentos, config->MAX_MOVIMENTOS);
        n->individuo.aptidao = old->individuo.aptidao;
        n->individuo.pontuacao_movimentos = old->individuo.pontuacao_movimentos;
        n->individuo.desvio_padrao = old->individuo.desvio_padrao;
        n->individuo.distancia = old->individuo.distancia;
        n->individuo.colisoes = old->individuo.colisoes;
        n->individuo.passos_validos = old->individuo.passos_validos;
        n->proximo = NULL;
        if (!nova) nova = cauda = n;
        else {
            cauda->proximo = n;
            cauda = n;
        }
        old = old->proximo;
    }

    // Gera novos individuos
    int cnt = elite_cnt;
    while (cnt < pop_size) {
        NoPopulacao *p1 = selecao_por_torneio(*cabeca, config);
        NoPopulacao *p2 = selecao_por_torneio(*cabeca, config);
        if (!p1 || !p2) {
            printf("Erro: Selecao de pais falhou\n");
            break;
        }

        NoPopulacao *filho1 = malloc(sizeof(NoPopulacao));
        NoPopulacao *filho2 = malloc(sizeof(NoPopulacao));
        if (!filho1 || !filho2) {
            printf("Erro: Falha ao alocar filhos\n");
            if (filho1) free(filho1);
            if (filho2) free(filho2);
            break;
        }

        cruzamento(&p1->individuo, &p2->individuo,
                  &filho1->individuo, &filho2->individuo, config);
        mutar(&filho1->individuo, config);
        mutar(&filho2->individuo, config);
        calcular_aptidao(&filho1->individuo, config);
        calcular_aptidao(&filho2->individuo, config);

        filho1->proximo = NULL;
        filho2->proximo = NULL;

        if (!nova) nova = cauda = filho1;
        else {
            cauda->proximo = filho1;
            cauda = filho1;
        }
        cnt++;

        if (cnt < pop_size) {
            cauda->proximo = filho2;
            cauda = filho2;
            cnt++;
        } else {
            if (filho2->individuo.movimentos)
                free(filho2->individuo.movimentos);
            free(filho2);
        }
    }

    liberar_populacao(*cabeca);
    *cabeca = nova;
}

void liberar_populacao(NoPopulacao *cabeca) {
    while (cabeca) {
        NoPopulacao *t = cabeca;
        cabeca = cabeca->proximo;
        if (t->individuo.movimentos)
            free(t->individuo.movimentos);
        free(t);
    }
}