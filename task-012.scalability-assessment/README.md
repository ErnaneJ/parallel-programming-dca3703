# Avaliação de Escalabilidade em Código de Difusão de Calor 3D (Navier-Stokes)

Este repositório contém a implementação e análise de escalabilidade de um código de simulação 3D de difusão de calor (Navier-Stokes) usando OpenMP. O objetivo é avaliar o desempenho do código com diferentes configurações de threads, tanto para escalabilidade forte quanto para escalabilidade fraca, no supercomputador da UFRN (NPAD).

## Descrição

O código implementa uma simulação de difusão de calor 3D, onde o domínio é discretizado em uma grade de tamanho `N x N x N` e evolui ao longo de `NSTEPS` iterações. A implementação utiliza OpenMP para paralelização e realiza duas avaliações de escalabilidade:

* **Escalabilidade Forte**: O problema possui tamanho fixo (N=256) e o tempo de execução é avaliado conforme o número de threads aumenta.
* **Escalabilidade Fraca**: O número de pontos no domínio cresce proporcionalmente ao número de threads, mantendo o volume total constante. O tempo de execução é avaliado para diferentes tamanhos de problema e número de threads.

## Metodologia

### Código de Escalabilidade Forte

O código de escalabilidade forte é executado mantendo o tamanho da grade `N` fixo, enquanto varia o número de threads. As execuções são feitas com 1, 2, 4, 8, 16 e 24 threads. Abaixo está o fluxo do programa:

1. Inicializa a grade de dados.
2. Executa a simulação de difusão de calor por `NSTEPS` iterações.
3. Avalia o tempo de execução com diferentes configurações de threads.

### Código de Escalabilidade Fraca

O código de escalabilidade fraca ajusta o tamanho do problema `N` conforme o número de threads, garantindo que o volume de trabalho por thread seja constante. O número de threads e o tamanho de `N` são os seguintes:

* 1 thread → N=32
* 2 threads → N=40
* 4 threads → N=51
* 8 threads → N=64
* 16 threads → N=80
* 24 threads → N=91

O programa realiza a simulação da difusão de calor para cada configuração de `N` e número de threads.

## Execução

Para rodar o código, utilize o script apropriado para cada tipo de escalabilidade. Existem dois scripts:

### 1. Escalabilidade Forte

Execute o script `strong-scalability-job.sh` para avaliar a escalabilidade forte.

```bash
sbatch strong-scalability-job.sh
```

### 2. Escalabilidade Fraca

Execute o script `weak-scalability-job.sh` para avaliar a escalabilidade fraca.

```bash
sbatch weak-scalability-job.sh
```

## Resultados

Os resultados obtidos a partir das execuções são registrados em arquivos de saída e podem ser analisados em formato CSV. Exemplos de arquivos gerados:

### Escalabilidade Forte

```csv
N,NSTEPS,Threads,Time (s)
256,1000,1,206.182
256,1000,2,181.725
256,1000,4,183.169
256,1000,8,183.608
256,1000,16,184.781
256,1000,24,185.823
```

### Escalabilidade Fraca

```csv
N,NSTEPS,Threads,Time (s)
32,1000,1,0.610
40,1000,2,1.197
51,1000,4,2.641
64,1000,8,4.764
80,1000,16,8.399
91,1000,24,11.920
```

## Conclusão

A análise de escalabilidade mostrou que a solução não apresenta escalabilidade eficiente, tanto para a escalabilidade forte quanto para a fraca. Melhorias são possíveis, como otimizações no acesso à memória e no balanceamento de threads.
