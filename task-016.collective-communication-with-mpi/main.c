#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h> 

#define MASTER 0

void fill_matrix(double *A, int M, int N, unsigned int seed)
{
  for (int i = 0; i < M * N; ++i)
  {
    A[i] = (double)(rand_r(&seed)) / RAND_MAX;
  }
}

void fill_vector(double *x, int N, unsigned int seed)
{
  for (int i = 0; i < N; ++i)
  {
    x[i] = (double)(rand_r(&seed)) / RAND_MAX;
  }
}

int main(int argc, char **argv)
{
  int rank, size;
  int M = 8; // Número de linhas da matriz A (padrão)
  int N = 4; // Número de colunas da matriz A (e tamanho do vetor x)

  MPI_Init(&argc, &argv); // Inicializa o MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

  // Permite passar M e N como argumentos via linha de comando
  if (argc >= 3) {
    M = atoi(argv[1]);
    N = atoi(argv[2]);
  }

  // Calcula o número de linhas que cada processo receberá (pode variar)
  int local_M = M / size;
  int remainder = M % size;

  // Processo 0 avisa se vai haver desequilíbrio
  if (rank == MASTER && remainder != 0) {
    printf("Aviso: número de linhas (%d) não é divisível por número de processos (%d), haverá desequilíbrio.\n", M, size);
  }

  // Determina quantas linhas cada processo realmente terá (alguns recebem +1 linha)
  int my_rows = (rank < remainder) ? local_M + 1 : local_M;

  // Alocação das estruturas de dados
  double *A = NULL; // Matriz A (completa, apenas no processo 0)
  double *x = (double *)malloc(N * sizeof(double)); // Vetor x
  double *local_A = (double *)malloc(my_rows * N * sizeof(double)); // Parte local de A
  double *local_y = (double *)malloc(my_rows * sizeof(double)); // Parte local de y
  double *y = NULL; // Vetor final y (apenas no processo 0)

  int *sendcounts = NULL;
  int *displs = NULL;
  int *recvcounts = NULL;
  int *recvdispls = NULL;

  if (rank == MASTER)
  {
    A = (double *)malloc(M * N * sizeof(double));
    y = (double *)malloc(M * sizeof(double));
    unsigned int seed = (unsigned int)(time(NULL) + rank * 100);
    fill_matrix(A, M, N, seed);
    fill_vector(x, N, seed + 999);

    // Configura quantas linhas cada processo receberá
    sendcounts = malloc(size * sizeof(int));
    displs = malloc(size * sizeof(int));
    recvcounts = malloc(size * sizeof(int));
    recvdispls = malloc(size * sizeof(int));

    int offset = 0;
    for (int i = 0; i < size; ++i)
    {
      int rows = (i < remainder) ? local_M + 1 : local_M;
      sendcounts[i] = rows * N;
      displs[i] = offset;
      recvcounts[i] = rows;
      recvdispls[i] = (i == 0) ? 0 : recvdispls[i - 1] + recvcounts[i - 1];
      offset += rows * N;
    }
  }

  // Broadcast do vetor x para todos os processos
  MPI_Bcast(x, N, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  // Distribuição das linhas da matriz A
  MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE,
               local_A, my_rows * N, MPI_DOUBLE,
               MASTER, MPI_COMM_WORLD);

  // Marca o início da execução paralela
  double t_start = MPI_Wtime();

  // Cada processo calcula sua parte do produto A*x
  for (int i = 0; i < my_rows; ++i)
  {
    local_y[i] = 0.0;
    for (int j = 0; j < N; ++j)
    {
      local_y[i] += local_A[i * N + j] * x[j];
    }
  }

  // Junta os vetores locais no processo mestre
  MPI_Gatherv(local_y, my_rows, MPI_DOUBLE,
              y, recvcounts, recvdispls, MPI_DOUBLE,
              MASTER, MPI_COMM_WORLD);

  // Marca o fim da execução paralela
  double t_end = MPI_Wtime();

  if (rank == MASTER)
  {
    // printf("Resultado y = A * x:\n");
    // for (int i = 0; i < M; ++i)
    // {
    //   printf("%f ", y[i]);
    // }
    // printf("\n");
    printf("Tempo total de execução: %f segundos\n", t_end - t_start);
  }

  // Liberação de memória
  if (rank == MASTER)
  {
    free(A);
    free(y);
    free(sendcounts);
    free(displs);
    free(recvcounts);
    free(recvdispls);
  }
  free(x);
  free(local_A);
  free(local_y);

  MPI_Finalize();
  return 0;
}
