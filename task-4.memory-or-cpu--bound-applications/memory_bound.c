#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

#define SIZE 500000000  // 4 GB com double

int main(int argc, char *argv[]) {
    struct timeval start, end;
    double elapsed_time;
    double *a, *b, *c;
    int num_threads = 1;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    a = (double *)malloc(SIZE * sizeof(double));
    b = (double *)malloc(SIZE * sizeof(double));
    c = (double *)malloc(SIZE * sizeof(double));

    if (!a || !b || !c) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    for (size_t i = 0; i < SIZE; i++) {
        a[i] = i * 1.0;
        b[i] = i * 2.0;
    }
    
    gettimeofday(&start, NULL);
    int stride = 16;
    size_t limit = SIZE / stride;
    #pragma omp parallel num_threads(num_threads)
    {
      #pragma omp for
      for (size_t i = 0; i < limit; i++)
      {
        size_t idx = i * stride; // Pula posições => baixa localidade
        c[idx] = a[idx] + b[idx];
      }
    }
    gettimeofday(&end, NULL);

    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Memory-bound with %d threads: %f seconds\n", num_threads, elapsed_time);
    fflush(stdout);

    free(a);
    free(b);
    free(c);

    return 0;
}
