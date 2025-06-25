# 🧬 Algoritmo Genético para Resolução de Labirintos

![Labirinto sendo resolvido](https://i.imgur.com/JQ8K0zG.gif)  
*Exemplo do algoritmo encontrando a solução para um labirinto*

## 📝 Descrição

Este projeto implementa um algoritmo genético para encontrar caminhos ótimos em labirintos. O sistema evolui populações de soluções candidatas (sequências de movimentos) através de seleção, cruzamento e mutação, utilizando uma função de aptidão baseada em:

- Distância até a saída
- Número de colisões com paredes
- Eficiência dos movimentos

## 🛠️ Como Executar

### Pré-requisitos
- Compilador C (gcc)
- Make (opcional)

### Compilação e Execução
```bash
gcc -o labirinto main.c config.c maze.c individual.c genetic.c -lm
./labirinto
