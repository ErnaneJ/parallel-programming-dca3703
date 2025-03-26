// Programa sequencial que multiplica uma matriz de qualquer tamanho, gerada aleatoriamente, por um vetor.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mach/mach_time.h>

#define SIZE 1000

void print_matrix(int matrix[SIZE][SIZE])
{
  int i, j;
  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
};

void print_vector(int vector[SIZE])
{
  int i;
  for (i = 0; i < SIZE; i++)
  {
    printf("%d ", vector[i]);
  }
  printf("\n\n");
};

int main()
{
  int i, j;
  int matrix[SIZE][SIZE];
  int vector[SIZE];
  int result[SIZE];

  for (i = 0; i < SIZE; i++)
  {
    for (j = 0; j < SIZE; j++)
    {
      matrix[i][j] = rand() % 10;
    }
    vector[i] = rand() % 10;
  }

  // printf("Matrix:\n");
  // print_matrix(matrix);

  // printf("Vector:\n");
  // print_vector(vector);

  mach_timebase_info_data_t info;
  double elapsed_time_ns;

  mach_timebase_info(&info);

  uint64_t start = mach_absolute_time();

  for (i = 0; i < SIZE; i++)
  {
    result[i] = 0;
    for (j = 0; j < SIZE; j++)
    {
      result[i] += matrix[i][j] * vector[j];
    }
  }

  uint64_t end = mach_absolute_time();

  // printf("Result:\n");
  // print_vector(result);
  
  uint64_t elapsed_ns = (end - start) * info.numer / info.denom;
  double elapsed_sec = elapsed_ns / 1e9;

  printf("Tempo de execução (parede): %f segundos\n", elapsed_sec);

  return 0;
}