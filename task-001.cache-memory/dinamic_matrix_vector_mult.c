/**
 * @file matrix_vector_mult.c
 * @brief Implements two versions of matrix-vector multiplication: row-major and column-major access.
 *
 * This program generates a random matrix and vector, performs multiplication
 * using both row-major and column-major access patterns, and measures execution
 * time for each method. The goal is to analyze performance differences based
 * on memory access patterns and cache utilization.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define SIZE 10000 ///< Default matrix and vector size.

/**
 * @brief Function to measure execution time of a function call.
 * @param func Function pointer to the multiplication function.
 * @param matrix Input matrix.
 * @param vector Input vector.
 * @param result Output vector.
 * @return Execution time in seconds.
 */
double measure_execution_time(void (*func)(int **, int *, int *), int **matrix, int *vector, int *result)
{
  struct timeval start, end;
  gettimeofday(&start, NULL);

  func(matrix, vector, result);

  gettimeofday(&end, NULL);

  double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  return elapsed;
}

/**
 * @brief Performs matrix-vector multiplication with row-major order.
 * @param matrix Input matrix.
 * @param vector Input vector.
 * @param result Output vector.
 */
void matrix_vector_multiplication_row(int **matrix, int *vector, int *result)
{
  for (int i = 0; i < SIZE; i++)
  {
    result[i] = 0;
    for (int j = 0; j < SIZE; j++)
    {
      result[i] += matrix[i][j] * vector[j];
    }
  }
}

/**
 * @brief Performs matrix-vector multiplication with column-major order.
 * @param matrix Input matrix.
 * @param vector Input vector.
 * @param result Output vector.
 */
void matrix_vector_multiplication_col(int **matrix, int *vector, int *result)
{
  for (int i = 0; i < SIZE; i++)
  {
    result[i] = 0;
  }
  for (int j = 0; j < SIZE; j++)
  {
    for (int i = 0; i < SIZE; i++)
    {
      result[i] += matrix[i][j] * vector[j];
    }
  }
}

/**
 * @brief Compares two vectors for equality.
 * @param vec1 First vector.
 * @param vec2 Second vector.
 * @return 1 if vectors are equal, 0 otherwise.
 */
int compare_vectors(int *vec1, int *vec2)
{
  for (int i = 0; i < SIZE; i++)
  {
    if (vec1[i] != vec2[i])
    {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Main function to initialize data and compare execution times.
 * @return 0 on successful execution.
 */
int main()
{
  printf("\n💡 Matrix-vector multiplication with SIZE=%d.\n\n", SIZE);

  int **matrix = (int **)malloc(SIZE * sizeof(int *));
  for (int i = 0; i < SIZE; i++)
  {
    matrix[i] = (int *)malloc(SIZE * sizeof(int));
  }
  int *vector = (int *)malloc(SIZE * sizeof(int));
  int *result_row = (int *)malloc(SIZE * sizeof(int));
  int *result_col = (int *)malloc(SIZE * sizeof(int));

  if (matrix == NULL || vector == NULL || result_row == NULL || result_col == NULL)
  {
    printf("> ❌ Memory allocation failed!\n");
    return 1;
  }
  else
  {
    printf("> ✅ Memory allocation success!\n");
  }

  srand(time(NULL));

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      matrix[i][j] = rand() % 10;
    }
    vector[i] = rand() % 10;
  }

  double time_row = measure_execution_time(matrix_vector_multiplication_row, matrix, vector, result_row);
  printf("\n- 🎯 Execution time (row-major): %f seconds\n", time_row);
  double time_col = measure_execution_time(matrix_vector_multiplication_col, matrix, vector, result_col);
  printf("- 🎯 Execution time (column-major): %f seconds\n\n", time_col);

  if (compare_vectors(result_row, result_col))
  {
    printf("> Results match! ✅\n\n");
  }
  else
  {
    printf("> Results do NOT match! ❌\n\n");
  }

  for (int i = 0; i < SIZE; i++)
  {
    free(matrix[i]);
  }
  free(matrix);
  free(vector);
  free(result_row);
  free(result_col);

  return 0;
}