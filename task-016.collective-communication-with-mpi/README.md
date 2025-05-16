# Produto Matriz-Vetor com MPI

Este projeto implementa uma versão paralela do produto entre uma matriz e um vetor, \( \mathbf{y} = \mathbf{A} \cdot \mathbf{x} \), utilizando MPI (Message Passing Interface). A aplicação foi desenvolvida em linguagem C e executada em ambiente de computação de alto desempenho (HPC) no NPAD/UFRN.

## 💡 Objetivo

Avaliar o desempenho da paralelização do produto matriz-vetor com MPI, dividindo a matriz entre os processos, distribuindo o vetor para todos, e reunindo os resultados localmente, comparando o tempo de execução em diferentes configurações.

## ⚙️ Funcionamento

1. A matriz \( \mathbf{A} \in \mathbb{R}^{M \times N} \) é dividida por linhas entre os processos com `MPI_Scatterv`.
2. O vetor \( \mathbf{x} \in \mathbb{R}^N \) é distribuído inteiro para todos os processos com `MPI_Bcast`.
3. Cada processo calcula sua parte do vetor \( \mathbf{y}^{(p)} = \mathbf{A}^{(p)} \cdot \mathbf{x} \).
4. O processo mestre reúne os resultados com `MPI_Gatherv`, formando o vetor completo \( \mathbf{y} \).

## 🧪 Execução no NPAD/UFRN

Para executar os experimentos no NPAD:

```bash
sbatch run-matvec.sbatch
````

Este script irá:

* Compilar o código com `mpicc`
* Rodar o programa com diferentes números de processos (2–32)
* Testar matrizes quadradas de 512×512 até 4096×4096
* Repetir cada teste 3 vezes e medir o tempo médio

## 📊 Análise dos Resultados

Utilize o script `plot_analysis.py` para gerar os gráficos de:

* Tempo médio de execução vs. tamanho da matriz
* Tempo médio de execução vs. número de processos
* Speedup vs. número de processos

Cada gráfico permite observar o impacto da paralelização no desempenho do algoritmo.

## 🧮 Representação Matemática

A operação paralelizada pode ser descrita por:

$$
\mathbf{y} = \bigcup_{p=0}^{P-1} \left( \mathbf{A}^{(p)} \cdot \mathbf{x} \right)
\quad \text{onde} \quad \mathbf{A}^{(p)} \in \mathbb{R}^{M_p \times N}
$$