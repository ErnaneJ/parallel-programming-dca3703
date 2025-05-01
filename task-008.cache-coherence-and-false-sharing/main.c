#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

#define N 9999999

/**
 * @brief Returns the current time in seconds.
 * 
 * This function uses the gettimeofday system call to obtain the current time with
 * microsecond precision.
 * 
 * @return The current time in seconds.
 */
double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1e6;
}

/**
 * @brief Version using rand() with #pragma omp critical.
 * 
 * This function estimates the value of Pi using a stochastic estimation method where
 * random points are generated and checked whether they fall inside the unit circle.
 * The results from each thread are accumulated in a global variable with the use of
 * a critical section to avoid race conditions.
 */
void version_critical_rand()
{
  double start = get_time();
  int count = 0;

  #pragma omp parallel
  {
    int local_count = 0;

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand() / RAND_MAX;
      double y = (double)rand() / RAND_MAX;
      if (x * x + y * y <= 1.0)
        local_count++;
    }

    #pragma omp critical
    count += local_count;
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🟡 Version 1 - critical + rand():      π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Version using rand() with a shared array.
 * 
 * This function estimates Pi in a similar way to version_critical_rand, but each thread
 * writes its local count of points inside the unit circle to a shared array. The accumulation
 * is done serially after the parallel section.
 */
void version_array_rand()
{
  double start = get_time();
  int threads = 0;
  int *counts = NULL;

  #pragma omp parallel
  {
    #pragma omp single
    {
      threads = omp_get_num_threads();
      counts = calloc(threads, sizeof(int));
    }
    int tid = omp_get_thread_num();

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand() / RAND_MAX;
      double y = (double)rand() / RAND_MAX;
      if (x * x + y * y <= 1.0)
        counts[tid]++;
    }
  }

  int total = 0;
  for (int i = 0; i < threads; i++)
    total += counts[i];
  free(counts);

  double pi = 4.0 * total / N;
  double elapsed = get_time() - start;
  printf("🔴 Version 2 - array + rand():         π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Version using rand_r() with #pragma omp critical.
 * 
 * This function is similar to version_critical_rand, but instead of rand(), it uses
 * rand_r() with a unique seed for each thread to avoid using a shared random number generator.
 */
void version_critical_rand_r()
{
  double start = get_time();
  int count = 0;

  #pragma omp parallel
  {
    int local_count = 0;
    unsigned int seed = time(NULL) ^ omp_get_thread_num();

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand_r(&seed) / RAND_MAX;
      double y = (double)rand_r(&seed) / RAND_MAX;
      if (x * x + y * y <= 1.0)
        local_count++;
    }

    #pragma omp critical
    count += local_count;
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🟢 Version 3 - critical + rand_r():    π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Version using rand_r() with a shared array.
 * 
 * This function is similar to version_array_rand, but instead of rand(), it uses
 * rand_r() with a unique seed for each thread to avoid using a shared random number generator.
 */
void version_array_rand_r()
{
  double start = get_time();
  int threads;
  int *counts;
  
  #pragma omp parallel
  {
    int tid = omp_get_thread_num();
    unsigned int seed = time(NULL) ^ tid;
    
    #pragma omp single
    {
      threads = omp_get_num_threads();
      counts = calloc(threads, sizeof(int));
    }

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand_r(&seed) / RAND_MAX;
      double y = (double)rand_r(&seed) / RAND_MAX;
      if (x * x + y * y <= 1.0)
        counts[tid]++;
    }
  }

  int total = 0;
  for (int i = 0; i < threads; i++)
    total += counts[i];
  free(counts);

  double pi = 4.0 * total / N;
  double elapsed = get_time() - start;
  printf("🌕 Version 4 - array + rand_r():       π ≈ %-15.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Main function that runs all the versions.
 * 
 * This function calls all the versions of the Pi estimation, printing the results and 
 * comparing execution times for each.
 * 
 * @return Returns 0 to indicate successful execution.
 */
int main()
{
  printf("==> Stochastic Pi Estimation (Task 8)\n\n");
  version_critical_rand();
  version_array_rand();
  version_critical_rand_r();
  version_array_rand_r();

  return 0;
}