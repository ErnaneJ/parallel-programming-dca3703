# Regiões Críticas Nomeadas e Locks Explícitos

Este projeto implementa um programa em C utilizando OpenMP para realizar inserções simultâneas em listas encadeadas, garantindo a integridade dos dados ao evitar condições de corrida. O programa é estruturado em dois modos:

1. **Modo Nomeado**: Utiliza regiões críticas nomeadas para gerenciar duas listas encadeadas.
2. **Modo Generalizado**: Permite um número variável de listas, usando locks explícitos para garantir a integridade dos dados.

## Objetivo

O objetivo principal deste projeto foi implementar e demonstrar o uso de **regiões críticas nomeadas** e **locks explícitos** para sincronização de threads em um programa paralelo. As tarefas paralelizadas realizam inserções em várias listas encadeadas, com o uso de OpenMP para gerenciar as threads. O projeto também explora as limitações de **regiões críticas nomeadas** e a necessidade de **locks explícitos** quando se lida com múltiplas listas.

## Descrição do Projeto

### Modos Implementados

1. **Modo Nomeado (Named Mode)**:
   - Neste modo, o programa cria duas listas encadeadas e utiliza regiões críticas nomeadas para inserir elementos nessas listas de forma concorrente.
   - As regiões críticas são usadas para garantir que a inserção em uma lista não afete a outra.

2. **Modo Generalizado (Generalized Mode)**:
   - Neste modo, o programa permite que o número de listas seja definido pelo usuário. As inserções são feitas de forma concorrente em listas diferentes utilizando **locks explícitos** para garantir a integridade dos dados.
   - O uso de locks é necessário para evitar condições de corrida quando o número de listas é grande e dinâmico, o que não seria possível apenas com regiões críticas nomeadas.

### Fluxo do Programa

- O programa começa com a execução do **Modo Nomeado**, onde duas listas são manipuladas em paralelo.
- Em seguida, o **Modo Generalizado** é ativado, permitindo ao usuário definir o número de listas e o número de inserções. As inserções são distribuídas de maneira aleatória entre as listas.
- Em ambos os modos, as listas são exibidas ao final, mostrando os valores inseridos de forma concorrente.

### Código

O código é implementado em **C** com o uso de **OpenMP** para paralelização. As funções principais incluem:

- **insert()**: Função para inserir um valor na lista encadeada.
- **print_list()**: Função para exibir o conteúdo das listas.
- **named_mode()**: Implementa o Modo Nomeado utilizando regiões críticas nomeadas.
- **generalized_mode()**: Implementa o Modo Generalizado utilizando locks explícitos.

## Resultados

A seguir estão os resultados obtidos durante a execução do programa:

### Modo Nomeado

```text
==== NAMED MODE (two lists, OpenMP tasks) ====
[Method Named] List 1: 10 -> 40 -> 10 -> 24 -> 89 -> 89 -> 75 -> NULL
[Method Named] List 2: 96 -> 33 -> 17 -> NULL
```

### Modo Generalizado

```text
==== GENERALIZED MODE (M lists, OpenMP tasks + locks) ====
Enter the number of lists (M, up to 100): 15
Enter the number of insertions: 100
[Method Generalized] List 0: 262 -> 139 -> 139 -> 158 -> 526 -> 403 -> NULL
[Method Generalized] List 1: 999 -> 999 -> 367 -> 367 -> 244 -> 18 -> 895 -> 263 -> NULL
[Method Generalized] List 2: 613 -> 104 -> 104 -> 981 -> 981 -> 472 -> 123 -> 0 -> 491 -> 368 -> NULL
...
```

## Compilação e Execução

```bash
gcc-14 -fopenmp ./task-9.named-critical-regions-and-explicit-locks/main.c -o ./task-9.named-critical-regions-and-explicit-locks/out/main.o && ./task-9.named-critical-regions-and-explicit-locks/out/main.o
```

Durante a execução, o programa solicitará o número de listas e o número de inserções a serem realizadas no **Modo Generalizado**.

## Conclusões

O projeto mostrou que o uso de **regiões críticas nomeadas** é eficaz para garantir a integridade dos dados em cenários com poucas listas. No entanto, à medida que o número de listas aumenta, o uso de **locks explícitos** se torna essencial para evitar condições de corrida, apesar da sobrecarga associada.

O uso de OpenMP foi fundamental para demonstrar as vantagens da paralelização e sincronização de tarefas em um contexto de múltiplas listas encadeadas.