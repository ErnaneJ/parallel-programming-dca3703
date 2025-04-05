#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

#define N 1000000000  // 1 bilhão

int main(int argc, char *argv[]) {
    struct timeval start, end;
    double elapsed_time;
    int num_threads = 1;

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    double result = 0.0;
    gettimeofday(&start, NULL);
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (long i = 1; i <= N; i++) {
            double x = sin(i) * log(i + 1) + cos(i) * exp(fmod(i, 1000));
            x = pow(x, 0.5) + sqrt(x + 1.0);
            result += x / (i + 1);
        }
    }
    gettimeofday(&end, NULL);

    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
    printf("Compute-bound with %d threads: %f seconds (result: %f)\n", num_threads, elapsed_time, result);
    fflush(stdout);

    return 0;
}
