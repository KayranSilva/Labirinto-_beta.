
# Genetic Labirinto Solver

[![Logo do Projeto](assets/logo.png)](https://github.com/seuusuario/genetic-labirinto-solver)

## Sobre
O **Genetic Labirinto Solver** é um projeto em C que utiliza algoritmos genéticos para encontrar soluções em labirintos, evoluindo sequências de movimentos (C, B, E, D) para sair do labirinto.

## Funcionalidades
- **Carregamento Dinâmico:** O labirinto é lido de um arquivo (definido em `config.ini`).
- **Algoritmo Genético:** Seleção por torneio, cruzamento e mutação com elitismo.
- **Registro de Progresso:** Geração de log em CSV (`log.csv`) com ID da geração, Fitness e Movimentos.
- **Visualização:** Exibe passo a passo o caminho do melhor indivíduo.

## Estrutura do Projeto

/GeneticLabirintoSolver ├── config.ini         # Parâmetros do algoritmo e do labirinto ├── maze.txt           # Arquivo do labirinto ├── log.csv            # Log gerado em cada execução ├── src │   ├── config.h / config.c │   ├── maze.h / maze.c │   ├── individual.h / individual.c │   ├── genetic.h / genetic.c │   └── main.c └── assets ├── logo.png ├── evolucao_fitness.png   # Exemplo de gráfico de evolução └── sample_maze.png        # Exemplo do labirinto
