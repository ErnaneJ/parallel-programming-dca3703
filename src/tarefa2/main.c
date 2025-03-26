#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 100000000

struct execution_result
{
  double time;
  int result;
};

void initialize_array(int *arr)
{
  for (int i = 0; i < N; i++)
  {
    arr[i] = i + 2;
  }
}

int sum_dependent(int *arr)
{
  int sum = 0;
  for (int i = 0; i < N; i++)
  {
    sum += arr[i];
  }

  return sum;
}

int sum_independent_2(int *arr) {
  int sum1 = 0, sum2 = 0;

  for (int i = 0; i < N; i += 2) {
    sum1 += arr[i];
    sum2 += arr[i + 1];
  }

  return sum1 + sum2;
}

int sum_independent_4(int *arr) {
  int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

  for (int i = 0; i < N; i += 4) {
    sum1 += arr[i];
    sum2 += arr[i + 1];
    sum3 += arr[i + 2];
    sum4 += arr[i + 3];
  }

  return sum1 + sum2 + sum3 + sum4;
}

int sum_independent_8(int *arr) {
  int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0;

  for (int i = 0; i < N; i += 8) {
    sum1 += arr[i];
    sum2 += arr[i + 1];
    sum3 += arr[i + 2];
    sum4 += arr[i + 3];
    sum5 += arr[i + 4];
    sum6 += arr[i + 5];
    sum7 += arr[i + 6];
    sum8 += arr[i + 7];
  }

  return sum1 + sum2 + sum3 + sum4 + sum5 + sum6 + sum7 + sum8;
}

struct execution_result measure_sum_time(int (*func)(int *), int *arr)
{
  struct timeval start, end;
  struct execution_result result;
  gettimeofday(&start, NULL);
  result.result = func(arr);
  gettimeofday(&end, NULL);
  result.time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  return result;
}

int main()
{
  int *arr = (int *)malloc(N * sizeof(int));
  if (!arr)
  {
    printf("Erro ao alocar memória!\n");
    return 1;
  }

  struct timeval start, end;
  gettimeofday(&start, NULL);
  initialize_array(arr);
  gettimeofday(&end, NULL);
  double init_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
  printf("Tempo de inicialização: %.6f segundos\n", init_time);

  struct execution_result dep_result = measure_sum_time(sum_dependent, arr);
  printf("Tempo (com dependência): %.6f segundos, Resultado: %d\n", dep_result.time, dep_result.result);
  
  struct execution_result indep_result_2 = measure_sum_time(sum_independent_2, arr);
  printf("Tempo (sem dependência - 2): %.6f segundos, Resultado: %d\n", indep_result_2.time, indep_result_2.result);
  
  struct execution_result indep_result_4 = measure_sum_time(sum_independent_4, arr);
  printf("Tempo (sem dependência - 4): %.6f segundos, Resultado: %d\n", indep_result_4.time, indep_result_4.result);
  
  struct execution_result indep_result_8 = measure_sum_time(sum_independent_8, arr);
  printf("Tempo (sem dependência - 8): %.6f segundos, Resultado: %d\n", indep_result_8.time, indep_result_8.result);

  int is_correct = dep_result.result == indep_result_2.result && dep_result.result == indep_result_4.result && dep_result.result == indep_result_8.result;
  printf("\nResultado consistente para todas as versões: %s\n", is_correct ? "Sim" : "Não");
  
  free(arr);
  return 0;
}
