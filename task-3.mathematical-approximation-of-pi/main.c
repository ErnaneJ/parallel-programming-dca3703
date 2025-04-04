#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

/**
 * @brief Counts the number of correct decimal places between the approximated and real values of PI.
 *
 * This function compares the first 15 decimal places of the approximated and real values of PI.
 *
 * @param approx_pi The approximated value of PI.
 * @param real_pi The real value of PI.
 * @return The number of correct decimal places.
 */
int count_correct_decimal_places(double approx_pi, double real_pi)
{
  char str_approx[50], str_real[50];
  snprintf(str_approx, sizeof(str_approx), "%.15f", approx_pi);
  snprintf(str_real, sizeof(str_real), "%.15f", real_pi);

  int count = 0;
  for (int i = 0; i < 15; i++)
  {
    if (str_approx[i] == str_real[i])
      count++;
    else
      break;
  }
  return count;
}

/**
 * @brief Calculates the approximation of PI using the Leibniz series.
 *
 * This function iterates over the Leibniz series and records the data in the provided file.
 *
 * @param iterations A pointer to the current number of correct decimal places.
 * @param file The file where the results will be saved in CSV format.
 */
double calculate_pi(int *iterations, FILE *file)
{
  double approx_pi = 0.0;
  int i = 0;

  struct timeval start, end;
  gettimeofday(&start, NULL);

  while (1)
  {
    double term = (i % 2 == 0 ? 1.0 : -1.0) / (2 * i + 1);
    approx_pi += term;
    i++;

    double current_pi = approx_pi * 4.0;
    int correct_places = count_correct_decimal_places(current_pi, M_PI);

    if (correct_places > *iterations)
    {
      gettimeofday(&end, NULL);

      double execution_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
      double error = fabs(M_PI - current_pi);

      printf("%10d | %18.15f | %13.10f | %19.6f | %10d\n", i, current_pi, error, execution_time, correct_places);
      fprintf(file, "%d,%.15f,%.10f,%.6f,%d\n", i, current_pi, error, execution_time, correct_places);
      fflush(file); // Force immediate writing to file
      *iterations = correct_places;
    }
  }
}

/**
 * @brief Main function to execute the PI approximation and write the results to a CSV file.
 *
 * This function opens a file for writing, calculates the approximation of PI using the
 * Leibniz series, and writes the results to the file in CSV format.
 *
 * @return Returns 0 on successful execution.
 */
int main()
{
  FILE *file = fopen("./task-3.mathematical-approximation-of-pi/data/pi_approximation.csv", "w");
  if (!file)
  {
    printf("Error opening file!\n");
    return 1;
  }

  fprintf(file, "Iterations,Approximation of PI,Absolute Error,Execution Time (s),Correct Decimal Places\n");

  printf(" Iterations | Approximation of PI | Absolute Error | Execution Time (s) | Correct Decimal Places\n");
  printf("----------------------------------------------------------------------------------------------\n");

  int correct_places = -1;
  calculate_pi(&correct_places, file);

  fclose(file);
  printf("Data successfully written to pi_approximation.csv\n");

  return 0;
}
