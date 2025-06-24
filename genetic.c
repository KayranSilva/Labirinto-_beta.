#include "genetic.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "individual.h"
#include "config.h"
int tamanho_populacao(NoPopulacao *cabeca) {
    int tamanho = 0;
    while(cabeca != NULL) {
        tamanho++;
        cabeca = cabeca->proximo;
    }
    return tamanho;
}

void inicializar_populacao(NoPopulacao **cabeca, Configuracoes *config) {
    *cabeca = NULL;
    NoPopulacao *cauda = NULL;
    
    for(int i = 0; i < config->TAM_POPULACAO; i++) {
        NoPopulacao *novo_no = (NoPopulacao*)malloc(sizeof(NoPopulacao));
        if(novo_no == NULL) {
            printf("Erro ao alocar memoria para novo individuo\n");
            liberar_populacao(*cabeca);
            exit(1);
        }

        // Aloca movimentos dinamicamente
        
        if(novo_no->individuo.movimentos == NULL) {
            printf("Erro ao alocar memoria para movimentos\n");
            free(novo_no);
            liberar_populacao(*cabeca);
            exit(1);
        }
        
        char ultimo_movimento = '\0';
        for(int j = 0; j < config->MAX_MOVIMENTOS; j++) {
            char movimento;
            int tentativas = 0;
            
            do {
                int r = rand() % 4;
                char movimentos[] = {'C', 'B', 'E', 'D'};
                movimento = movimentos[r];
                tentativas++;
                
                if(!movimento_redundante(ultimo_movimento, movimento) || tentativas > 2) {
                    break;
                }
            } while(1);
            
            novo_no->individuo.movimentos[j] = movimento;
            ultimo_movimento = movimento;
        }
        
        calcular_aptidao(&novo_no->individuo, config);
        novo_no->proximo = NULL;
        
        if(*cabeca == NULL) {
            *cabeca = novo_no;
            cauda = novo_no;
        } else {
            cauda->proximo = novo_no;
            cauda = novo_no;
        }
    }
}

NoPopulacao* selecao_por_torneio(NoPopulacao *cabeca, Configuracoes *config) {
    if(cabeca == NULL) return NULL;
    
    NoPopulacao *melhor = NULL;
    float melhor_aptidao = -1000;
    int tam_pop = tamanho_populacao(cabeca);
    
    for(int i = 0; i < config->TAM_TORNEIO; i++) {
        int indice = rand() % tam_pop;
        NoPopulacao *atual = cabeca;
        
        for(int j = 0; j < indice && atual != NULL; j++) {
            atual = atual->proximo;
        }
        
        if(atual != NULL) {
            if(melhor == NULL || atual->individuo.aptidao > melhor_aptidao) {
                melhor = atual;
                melhor_aptidao = atual->individuo.aptidao;
            }
        }
    }
    return melhor;
}

void cruzamento(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2, Configuracoes *config) {
    if((double)rand()/RAND_MAX > config->TAXA_CRUZAMENTO) {
        memcpy(filho1, pai1, sizeof(Individuo));
        memcpy(filho2, pai2, sizeof(Individuo));
        return;
    }

    int ponto1 = rand() % config->MAX_MOVIMENTOS;
    int ponto2 = rand() % config->MAX_MOVIMENTOS;
    if(ponto1 > ponto2) {
        int temp = ponto1;
        ponto1 = ponto2;
        ponto2 = temp;
    }
    
    for(int i = 0; i < ponto1; i++) {
        filho1->movimentos[i] = pai1->movimentos[i];
        filho2->movimentos[i] = pai2->movimentos[i];
    }
    
    for(int i = ponto1; i < ponto2; i++) {
        filho1->movimentos[i] = pai2->movimentos[i];
        filho2->movimentos[i] = pai1->movimentos[i];
    }
    
    for(int i = ponto2; i < config->MAX_MOVIMENTOS; i++) {
        filho1->movimentos[i] = pai1->movimentos[i];
        filho2->movimentos[i] = pai2->movimentos[i];
    }
}

void mutar(Individuo *ind, Configuracoes *config) {
    char ultimo_movimento = '\0';
    
    for(int i = 0; i < config->MAX_MOVIMENTOS; i++) {
        if((double)rand()/RAND_MAX < config->TAXA_MUTACAO) {

            if(ind->distancia < 3 && i < 16) {
                char movimentos_para_saida[4];
                int contador = 0;
                if(saida_x > inicio_x) movimentos_para_saida[contador++] = 'B';
                if(saida_x < inicio_x) movimentos_para_saida[contador++] = 'C';
                if(saida_y > inicio_y) movimentos_para_saida[contador++] = 'D';
                if(saida_y < inicio_y) movimentos_para_saida[contador++] = 'E';
                
                if(contador > 0) {
                    ind->movimentos[i] = movimentos_para_saida[rand() % contador];
                    ultimo_movimento = ind->movimentos[i];
                    continue;
                }
            }
            
     
            char novo_movimento;
            int tentativas = 0;
            
            do {
                int r = rand() % 4;
                char movimentos[] = {'C', 'B', 'E', 'D'};
                novo_movimento = movimentos[r];
                tentativas++;
                
                if(!movimento_redundante(ultimo_movimento, novo_movimento) || tentativas > 2) {
                    break;
                }
            } while(1);
            
            ind->movimentos[i] = novo_movimento;
        }
        
        ultimo_movimento = ind->movimentos[i];
    }
}

void ordenar_populacao(NoPopulacao **cabeca) {
    if(*cabeca == NULL || (*cabeca)->proximo == NULL) return;
    
    NoPopulacao *ordenada = NULL;
    NoPopulacao *atual = *cabeca;
    
    while(atual != NULL) {
        NoPopulacao *proximo = atual->proximo;
        
        if(ordenada == NULL || atual->individuo.aptidao > ordenada->individuo.aptidao) {
            atual->proximo = ordenada;
            ordenada = atual;
        } else {
            NoPopulacao *temp = ordenada;
            while(temp->proximo != NULL && 
                  atual->individuo.aptidao <= temp->proximo->individuo.aptidao) {
                temp = temp->proximo;
            }
            atual->proximo = temp->proximo;
            temp->proximo = atual;
        }
        atual = proximo;
    }
    
    *cabeca = ordenada;
}

void nova_geracao(NoPopulacao **cabeca, Configuracoes *config) {
    if(*cabeca == NULL) return;

    ordenar_populacao(cabeca);
    
    NoPopulacao *nova_cabeca = NULL;
    NoPopulacao *nova_cauda = NULL;
    int elite = config->TAM_POPULACAO * config->TAXA_ELITISMO;
    int tamanho_atual = 0;
    

    NoPopulacao *atual = *cabeca;
    for(int i = 0; i < elite && atual != NULL; i++) {
        NoPopulacao *novo_no = (NoPopulacao*)malloc(sizeof(NoPopulacao));
        if(novo_no == NULL) {
            printf("Erro ao alocar memoria para elite\n");
            liberar_populacao(nova_cabeca);
            exit(1);
        }
        
        memcpy(&novo_no->individuo, &atual->individuo, sizeof(Individuo));
        novo_no->proximo = NULL;
        
        if(nova_cabeca == NULL) {
            nova_cabeca = novo_no;
            nova_cauda = novo_no;
        } else {
            nova_cauda->proximo = novo_no;
            nova_cauda = novo_no;
        }
        atual = atual->proximo;
        tamanho_atual++;
    }
    

    while(tamanho_atual < config->TAM_POPULACAO) {
        NoPopulacao *pai1 = selecao_por_torneio(*cabeca, config);
        NoPopulacao *pai2 = selecao_por_torneio(*cabeca, config);
        
        if(pai1 == NULL || pai2 == NULL) {
            printf("Erro na selecao de pais\n");
            break;
        }
        
        NoPopulacao *filho1 = (NoPopulacao*)malloc(sizeof(NoPopulacao));
        NoPopulacao *filho2 = (NoPopulacao*)malloc(sizeof(NoPopulacao));
        if(filho1 == NULL || filho2 == NULL) {
            printf("Erro ao alocar memoria para filhos\n");
            free(filho1);
            free(filho2);
            break;
        }
        
        cruzamento(&pai1->individuo, &pai2->individuo, 
                 &filho1->individuo, &filho2->individuo, config);
        
        mutar(&filho1->individuo, config);
        mutar(&filho2->individuo, config);
        
        calcular_aptidao(&filho1->individuo, config);
        calcular_aptidao(&filho2->individuo, config);
        
        filho1->proximo = NULL;
        filho2->proximo = NULL;
        
        if(nova_cabeca == NULL) {
            nova_cabeca = filho1;
            nova_cauda = filho1;
        } else {
            nova_cauda->proximo = filho1;
            nova_cauda = filho1;
        }
        tamanho_atual++;
        
        if(tamanho_atual < config->TAM_POPULACAO) {
            nova_cauda->proximo = filho2;
            nova_cauda = filho2;
            tamanho_atual++;
        } else {
            free(filho2);
        }
    }
    
    liberar_populacao(*cabeca);
    *cabeca = nova_cabeca;
}

void liberar_populacao(NoPopulacao *cabeca) {
    while(cabeca != NULL) {
        NoPopulacao *temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }
}