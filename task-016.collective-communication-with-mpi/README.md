# 📘 Produto Matriz-Vetor com MPI

## 🧩 Objetivo

Este projeto implementa a multiplicação de uma matriz $A$ por um vetor $x$, resultando em $y = A \cdot x$, usando **MPI (Message Passing Interface)** para paralelizar o cálculo entre múltiplos processos.

O foco principal está em:

* Dividir o trabalho de forma **balanceada** (quando possível) ou **desbalanceada** (quando necessário);
* Avaliar o desempenho da aplicação com **diferentes tamanhos de matriz** e **número de processos**;
* Estudar o **impacto da comunicação coletiva MPI** na eficiência paralela;
* Medir corretamente o **tempo total de execução**, incluindo **comunicação e cálculo**.

## 🖥️ Estrutura do Código

### 🔨 Compilação

Compile com `mpicc`, usando otimização:

```bash
mpicc -O3 -o matvec main.c
```

### ▶️ Execução via SLURM

Um script SLURM automatiza os testes para diferentes tamanhos de matriz e números de processos:

```bash
sbatch script_job.sh
```

O script testa execuções com 2, 4, 8, 16 e 32 processos, em matrizes de tamanhos $512 \times 512$ até $4096 \times 4096$.

## 🚀 Lógica da Paralelização

### 1. **Distribuição da matriz `A`**

* A matriz é dividida **por linhas**.
* Como $M$ (número de linhas) pode **não ser divisível** por $P$ (número de processos), é usada:

> ✅ `MPI_Scatterv`: permite distribuir **quantidades variáveis** de linhas por processo.
> ⛔ `MPI_Scatter` exige divisão uniforme, o que causaria erro ou ineficiência.

### 2. **Distribuição do vetor `x`**

> ✅ `MPI_Bcast`: utilizado para **enviar o vetor `x` completo para todos os processos**, já que todos precisam dele para calcular o produto local.

### 3. **Cálculo local**

Cada processo computa o produto escalar das linhas de sua fatia da matriz com o vetor `x`, produzindo uma parte do vetor `y`.

### 4. **Recolhimento dos resultados**

> ✅ `MPI_Gatherv`: usado para **reunir partes variáveis do vetor `y`** de volta no processo mestre.
> ⛔ `MPI_Gather` não seria suficiente, pois assume tamanhos iguais de dados a serem coletados.

## 📈 Gráficos e Análise de Resultados

Os resultados foram obtidos com medições reais de tempo (incluindo **toda a comunicação MPI**) e analisados estatisticamente. Os gráficos abaixo foram gerados com Python, usando os dados médios de 3 execuções por configuração.

### 1. **Speedup vs. Número de Processos**

![Speedup](./speedup-vs-numero-de-processos.png)

* Para **matrizes grandes**, o speedup aumenta com o número de processos até certo ponto (\~16).
* Para **matrizes pequenas**, o uso excessivo de processos **piora o desempenho**, por causa da **sobrecarga de comunicação**.

### 2. **Tempo de Execução vs. Número de Processos**

![Tempo por processo](./tempo-de-execucao-vs-numero-de-processos.png)

* Redução clara de tempo para matrizes grandes (2048×2048+).
* Com **32 processos**, há saturação: o ganho computacional é anulado pela **latência e sincronização MPI**.

### 3. **Tempo de Execução vs. Tamanho da Matriz**

![Tempo vs Tamanho](./tempo-de-execucao-vs-tamanho-da-matriz.png)

* O tempo cresce aproximadamente **linearmente** com o número de elementos da matriz.
* Com mais processos, matrizes grandes são resolvidas mais rapidamente.
  Já para matrizes pequenas, o paralelismo **não compensa**.

## 📌 Comparativo: Por que `MPI_Scatterv` e `MPI_Gatherv`?

| Função         | Quando usar                                            | Usada neste projeto? |
| -------------- | ------------------------------------------------------ | -------------------- |
| `MPI_Scatter`  | Quando todos recebem **quantidades iguais**            | ❌                    |
| `MPI_Scatterv` | Quando cada processo recebe **quantidades diferentes** | ✅                    |
| `MPI_Gather`   | Quando todos enviam **quantidades iguais**             | ❌                    |
| `MPI_Gatherv`  | Quando cada processo envia **quantidades diferentes**  | ✅                    |
| `MPI_Bcast`    | Para enviar o mesmo dado a todos os processos          | ✅                    |