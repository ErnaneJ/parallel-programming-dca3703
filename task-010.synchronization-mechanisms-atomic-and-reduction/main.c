#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <omp.h>

#define N 999999999

/**
 * @brief Get the current wall-clock time in seconds.
 * 
 * @return Current time in seconds as a double.
 */
double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1e6;
}

/**
 * @brief Monte Carlo estimation of π using OpenMP with `#pragma omp critical` and local counting.
 *        Uses thread-local `rand_r()` generators and combines results in a critical section.
 */
void version_critical_with_local_count_rand_r()
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
  printf("🟢 [v1] critical + local count + rand_r():        π ≈ %.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Monte Carlo estimation of π using OpenMP with `#pragma omp atomic`.
 *        Each thread uses a local counter and updates a global total with atomic operations.
 */
void version_atomic_with_local_count_rand_r()
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

    #pragma omp atomic
    count += local_count;
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🔵 [v2] atomic + local count + rand_r():          π ≈ %.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Monte Carlo estimation of π using OpenMP `reduction` clause.
 *        Each thread accumulates results locally and OpenMP reduces the final result.
 */
void version_reduction_rand_r()
{
  double start = get_time();
  int count = 0;

  #pragma omp parallel reduction(+:count)
  {
    unsigned int seed = time(NULL) ^ omp_get_thread_num();

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand_r(&seed) / RAND_MAX;
      double y = (double)rand_r(&seed) / RAND_MAX;

      if (x * x + y * y <= 1.0)
        count++;
    }
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🟣 [v3] reduction(+:count) + rand_r():            π ≈ %.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Inefficient version: `rand_r()` used directly with `#pragma omp atomic` on each hit.
 *        Demonstrates high synchronization overhead.
 */
void worst_version_atomic_rand_r()
{
  double start = get_time();
  int count = 0;

  #pragma omp parallel
  {
    unsigned int seed = time(NULL) ^ omp_get_thread_num();

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand_r(&seed) / RAND_MAX;
      double y = (double)rand_r(&seed) / RAND_MAX;

      if (x * x + y * y <= 1.0)
      {
        #pragma omp atomic
        count++;
      }
    }
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🔻 [v4] worst: atomic per hit + rand_r():         π ≈ %.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Inefficient version: `rand_r()` with local count but final addition done using `#pragma omp critical`.
 *        Inefficient due to unnecessary use of critical with small updates.
 */
void worst_version_critical_rand_r()
{
  double start = get_time();
  int count = 0;

  #pragma omp parallel
  {
    unsigned int seed = time(NULL) ^ omp_get_thread_num();

    #pragma omp for
    for (int i = 0; i < N; i++)
    {
      double x = (double)rand_r(&seed) / RAND_MAX;
      double y = (double)rand_r(&seed) / RAND_MAX;

      if (x * x + y * y <= 1.0)
      {
        #pragma omp critical
        count++;
      }
    }
  }

  double pi = 4.0 * count / N;
  double elapsed = get_time() - start;
  printf("🔻 [v5] worst: critical per hit + rand_r():       π ≈ %.15f | Time: %.3fs\n", pi, elapsed);
}

/**
 * @brief Main function that executes and compares different implementations
 *        of Monte Carlo estimation for π using various OpenMP synchronization mechanisms.
 * 
 * @return int 
 */
int main()
{
  printf("== Monte Carlo π Estimation with OpenMP ==\n\n");
  version_critical_with_local_count_rand_r();
  version_atomic_with_local_count_rand_r();
  version_reduction_rand_r();
  worst_version_atomic_rand_r();
  worst_version_critical_rand_r();
  return 0;
}

// gcc-14 -fopenmp ./task-10.synchronization-mechanisms-atomic-and-reduction/main.c -o ./task-10.synchronization-mechanisms-atomic-and-reduction/out/main.o && ./task-10.synchronization-mechanisms-atomic-and-reduction/out/main.o