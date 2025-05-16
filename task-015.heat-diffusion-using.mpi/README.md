# Simulação Paralela da Difusão de Calor 1D com MPI

Este projeto implementa uma simulação da **difusão de calor em uma barra unidimensional (1D)** utilizando paralelismo com **MPI (Message Passing Interface)**. A barra é dividida entre múltiplos processos MPI, e cada processo é responsável por calcular a evolução da temperatura em sua seção da barra ao longo do tempo.

## Objetivo

Comparar o desempenho de três versões distintas de comunicação entre processos MPI:

1. **Versão 1 – Comunicação bloqueante:** utiliza `MPI_Send` / `MPI_Recv`
2. **Versão 2 – Comunicação não bloqueante + espera:** utiliza `MPI_Isend` / `MPI_Irecv` + `MPI_Waitall`
3. **Versão 3 – Comunicação não bloqueante com sobreposição:** utiliza `MPI_Isend` / `MPI_Irecv` + `MPI_Test` para calcular os pontos internos enquanto espera pelas bordas

## Estrutura do Código

- `main.c`: código principal contendo as três versões da simulação
  - `simulate_blocking(...)`: comunicação bloqueante
  - `simulate_nonblocking_wait(...)`: comunicação não bloqueante com espera
  - `simulate_nonblocking_test(...)`: comunicação não bloqueante com sobreposição

A simulação aplica a equação de difusão de calor:

```math
u[i] = u[i] + α * (u[i-1] - 2*u[i] + u[i+1])
```

## Configuração do Problema

* Tamanho da barra: `N = 10.000.000`
* Número de passos de tempo: `STEPS = 100.000`
* Coeficiente de difusão: `ALPHA = 0.01`

A barra começa com calor apenas no centro, e o calor se propaga ao longo do tempo. Cada processo armazena **ghost cells** para trocar fronteiras com os vizinhos.

## Execução no NPAD/UFRN

### Compilação

```bash
mpicc -O3 -o heat_diffusion main.c
```

### Execução com SLURM

Exemplo de script SLURM (`run-scaling-heatdiff.sh`):

```bash
#!/bin/bash
#SBATCH --job-name=HEATDIFF
#SBATCH --output=results-heatdiff-%j.out
#SBATCH --ntasks=32
#SBATCH --cpus-per-task=1
#SBATCH --time=01:00:00
#SBATCH --partition=intel-512

mpicc -O3 -o heat_diffusion main.c
EXEC=./heat_diffusion
export OMPI_MCA_btl="self,vader"

for P in 2 4 8 16 32; do
  echo "Executando com $P processos..."
  mpirun -np $P $EXEC
done
```

## Resultados

| Nº de Processos | Bloqueante | Não bloqueante + Wait | Não bloqueante + Test |
| --------------- | ---------- | --------------------- | --------------------- |
| 2               | 1138.36 s  | 1140.14 s             | 1144.12 s             |
| 4               | 746.66 s   | 746.57 s              | 745.03 s              |
| 8               | 685.91 s   | 685.95 s              | 693.82 s              |
| 16              | 686.11 s   | 684.51 s              | 689.70 s              |
| 32              | 687.53 s   | 687.63 s              | 680.97 s              |

## Funções e Constantes MPI Utilizadas

### Funções MPI

| Função          | Explicação                                                                           |
| --------------- | ------------------------------------------------------------------------------------ |
| `MPI_Send`      | Envia dados de forma **bloqueante** para outro processo.                             |
| `MPI_Recv`      | Recebe dados de forma **bloqueante** de outro processo.                              |
| `MPI_Isend`     | Inicia envio de dados de forma **não bloqueante**, continua execução sem esperar.    |
| `MPI_Irecv`     | Inicia recepção de dados de forma **não bloqueante**, continua execução sem esperar. |
| `MPI_Waitall`   | Espera todas as operações MPI pendentes (como `Isend/Irecv`) terminarem.             |
| `MPI_Test`      | Verifica se uma operação MPI não bloqueante terminou **sem travar** o processo.      |
| `MPI_Wait`      | Espera **uma única operação** MPI (como `Isend` ou `Irecv`) ser concluída.           |
| `MPI_Comm_rank` | Retorna o **rank (ID)** do processo atual dentro de um comunicador.                  |
| `MPI_Comm_size` | Retorna o **número total de processos** em um comunicador.                           |
| `MPI_Init`      | Inicializa o ambiente MPI; **deve ser chamada primeiro**.                            |
| `MPI_Finalize`  | Finaliza o ambiente MPI; **deve ser chamada por último**.                            |
| `MPI_Barrier`   | Sincroniza todos os processos — todos **esperam até que todos cheguem nesse ponto**. |
| `MPI_Wtime`     | Retorna o tempo atual em segundos — usado para medir **tempo de execução**.          |

### Constantes e Tipos MPI

| Constante / Tipo    | Explicação                                                                  |
| ------------------- | --------------------------------------------------------------------------- |
| `MPI_COMM_WORLD`    | Comunicador padrão que inclui **todos os processos MPI**.                   |
| `MASTER`            | Constante definida no código como `0`, representa o **processo principal**. |
| `MPI_DOUBLE`        | Tipo de dado usado na comunicação: um **`double` em C**.                    |
| `MPI_STATUS_IGNORE` | Informa ao MPI para **ignorar os detalhes da mensagem recebida**.           |
| `MPI_Request`       | Tipo usado para **rastrear operações não bloqueantes** (`Isend/Irecv`).     |
| `MPI_REQUEST_NULL`  | Valor especial indicando **"nenhuma operação válida"** no `MPI_Request`.    |

## Conclusão

A simulação demonstrou que a escolha da estratégia de comunicação pode impactar o desempenho de forma sutil em certos contextos. A sobreposição de comunicação e computação com `MPI_Test` é teoricamente vantajosa, mas seus ganhos práticos dependem da carga por processo e da latência de rede. Para cargas maiores ou configurações com menor número de processos, os benefícios podem se tornar mais evidentes.
