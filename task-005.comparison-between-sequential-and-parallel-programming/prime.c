/**
 * @file prime_benchmark.c
 * @brief Benchmark comparing sequential and parallel implementations for counting prime numbers.
 *
 * This program evaluates the performance of different approaches to counting prime numbers
 * up to a given integer `n`: a sequential implementation, a parallel implementation without
 * race condition handling (incorrect), and a parallel implementation using OpenMP `reduction`
 * (correct). Results are written to a CSV file for further analysis.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

/**
 * @brief Checks if a number is prime.
 *
 * @param num The number to check.
 * @return 1 if the number is prime, 0 otherwise.
 */
int is_prime(int num)
{
  if (num < 2)
    return 0;
  for (int i = 2; i * i <= num; i++)
  {
    if (num % i == 0)
      return 0;
  }
  return 1;
}

/**
 * @brief Calculates execution time in microseconds.
 *
 * @param start The start time.
 * @param end The end time.
 * @return Execution time in microseconds.
 */
long execution_time_us(struct timeval start, struct timeval end)
{
  return (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
}

/**
 * @brief Logs and prints the result of a test run.
 *
 * @param file Output file pointer.
 * @param n The input size.
 * @param threads Number of threads used.
 * @param version Version name (e.g., "sequential", "parallel_reduction").
 * @param total Total number of primes found.
 * @param time_us Execution time in microseconds.
 */
void write_result(FILE *file, int n, int threads, const char *version, int total, long time_us)
{
  double time_sec = time_us / 1e6;
  printf("n: %d, threads: %d, version: %s, total_primes: %d, time_seconds: %.6f\n",
         n, threads, version, total, time_sec);
  fprintf(file, "%d,%d,%s,%d,%.6f\n", n, threads, version, total, time_sec);
}

/**
 * @brief Runs all test versions for a given input size and number of threads.
 *
 * @param file Output file pointer.
 * @param n Upper limit to count primes.
 * @param threads Number of threads to use for parallel versions.
 */
void run_tests_for_n(FILE *file, int n, int threads)
{
  struct timeval start, end;
  long time;

  // Sequential version
  int total_primes_seq = 0;
  gettimeofday(&start, NULL);
  for (int i = 2; i <= n; i++)
  {
    if (is_prime(i))
      total_primes_seq++;
  }
  gettimeofday(&end, NULL);
  time = execution_time_us(start, end);
  write_result(file, n, threads, "sequential", total_primes_seq, time);

  // Parallel version without reduction (incorrect)
  int total_primes_wrong = 0;
  gettimeofday(&start, NULL);
#pragma omp parallel for num_threads(threads)
  for (int i = 2; i <= n; i++)
  {
    if (is_prime(i))
      total_primes_wrong++;
  }
  gettimeofday(&end, NULL);
  time = execution_time_us(start, end);
  write_result(file, n, threads, "parallel_no_reduction", total_primes_wrong, time);

  // Parallel version with reduction (correct)
  int total_primes_reduction = 0;
  gettimeofday(&start, NULL);
#pragma omp parallel for reduction(+ : total_primes_reduction) num_threads(threads)
  for (int i = 2; i <= n; i++)
  {
    if (is_prime(i))
      total_primes_reduction++;
  }
  gettimeofday(&end, NULL);
  time = execution_time_us(start, end);
  write_result(file, n, threads, "parallel_reduction", total_primes_reduction, time);
}

/**
 * @brief Main function that controls execution of all test cases and outputs results.
 */
int main()
{
  int max_threads = omp_get_max_threads();
  int thread_options[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int n_values[] = {4, 1000, 10000, 100000, 500000, 1000000, 5000000, 9999999};

  FILE *file = fopen("./task-5.comparison-between-sequential-and-parallel-programming/out/results.csv", "w");
  if (!file)
  {
    perror("Failed to open results.csv");
    return 1;
  }

  fprintf(file, "n,threads,version,total_primes,time_seconds\n");

  for (int i = 0; i < sizeof(n_values) / sizeof(int); i++)
  {
    for (int j = 0; j < sizeof(thread_options) / sizeof(int); j++)
    {
      if (thread_options[j] <= max_threads)
      {
        run_tests_for_n(file, n_values[i], thread_options[j]);
      }
    }
  }

  fclose(file);
  printf("✅ All results written to results.csv\n");

  return 0;
}
