#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso: %s <tamanho do array> <numero a buscar>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    if (n <= 0)
    {
        printf("O tamanho do array deve ser um inteiro positivo.\n");
        return 1;
    }

    srand(time(NULL));

    int *array = (int *)malloc(n * sizeof(int));
    if (array == NULL)
    {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 10;
    }

    int count_seq = 0;
    double start_seq = omp_get_wtime();

    for (int i = 0; i < n; i++)
    {
        if (array[i] == m)
        {
            count_seq++;
        }
    }

    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;

    printf("[Sequencial] O numero %d apareceu %d vezes no array de tamanho %d.\n", m, count_seq, n);
    printf("[Sequencial] Tempo de execução: %f segundos\n", time_seq);

    int count_par = 0;
    int num_threads = omp_get_max_threads();
    double start_par = omp_get_wtime();

    // #pragma parallel omp for
    // for (int i = 0; i < n; i++)
    // {
    //     if (array[i] == m)
    //     {
    //         #pragma omp critical
    //         count_par++;
    //     }
    // }

    #pragma omp parallel
    {
      int local_count_par = 0;
      #pragma omp for
      for (int i = 0; i < n; i++)
      {
          if (array[i] == m)
          {
              local_count_par++;
          }
      }

      #pragma omp critical
      count_par += local_count_par;
    }

    double end_par = omp_get_wtime();
    double time_par = end_par - start_par;

    printf("[OpenMP] O numero %d apareceu %d vezes no array de tamanho %d.\n", m, count_par, n);
    printf("[OpenMP] Tempo de execução: %f segundos\n", time_par);
    printf("[OpenMP] Número de threads usadas: %d\n", num_threads);

    int count_par2 = 0;
    int counts[num_threads];
    for (int i = 0; i < num_threads; i++) {
      counts[i] = 0;
    } 
    double start_par2 = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
      int tnum = omp_get_thread_num();
      #pragma omp for
      for (int i = 0; i < n; i++)
      {
          if (array[i] == m)
          {
              counts[tnum]++;
          }
      }

    }

    for(int i = 0; i < num_threads; i++)
    {
      count_par2 += counts[i];
    }

    double end_par2 = omp_get_wtime();
    double time_par2 = end_par2 - start_par2;

    printf("[OpenMP2] O numero %d apareceu %d vezes no array de tamanho %d.\n", m, count_par2, n);
    printf("[OpenMP2] Tempo de execução: %f segundos\n", time_par2);
    printf("[OpenMP2] Número de threads usadas: %d\n", num_threads);

    free(array);
    return 0;
}
