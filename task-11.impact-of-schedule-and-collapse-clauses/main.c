/**
 * @file main.c
 * @brief Simulates 3D heat diffusion with various OpenMP scheduling strategies.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <omp.h>
 
 #define N 32
 #define NSTEPS 10000
 #define DT 0.01f
 #define DX 1.0f
 #define VISC 0.1f
 #define SNAPSHOT_INTERVAL 10
 
 float u[N][N][N], u_new[N][N][N];
 
 /**
  * @brief Saves CSV header for benchmark logging.
  * @param f Output file pointer.
  */
 void save_csv_header(FILE *f);
 
 /**
  * @brief Initializes the 3D field with a single central perturbation.
  */
 void initialize() {
     memset(u, 0, sizeof(u));
     int cx = N / 2, cy = N / 2, cz = N / 2;
     u[cx][cy][cz] = 1.0f;
 }
 
 /**
  * @brief Saves a snapshot of the field to a binary file.
  * @param f Output binary file pointer.
  */
 void save_snapshot(FILE *f) {
     fwrite(u, sizeof(float), N * N * N, f);
 }
 
 void save_csv_header(FILE *f) {
     fprintf(f, "schedule_type,chunk_size,collapse,time_seconds\n");
 }
 
 /**
  * @brief Runs the fluid simulation with given OpenMP parameters.
  *
  * @param schedule_type "static", "dynamic", or "guided"
  * @param chunk_size Chunk size for the scheduler
  * @param collapse Whether to collapse nested loops (1 = yes, 0 = no)
  * @param log_file Output CSV file for performance data
  * @param bin_file Output binary file for snapshots
  */
 void run_simulation(const char *schedule_type, int chunk_size, int collapse, FILE *log_file, FILE *bin_file) {
     initialize();
     double start = omp_get_wtime();
 
     if (collapse == 1) {
         #pragma omp parallel for collapse(3) schedule(static, 1)
         for (int i = 1; i < N-1; i++)
             for (int j = 1; j < N-1; j++)
                 for (int k = 1; k < N-1; k++)
                     u_new[i][j][k] = 0.0f;
     }
 
     for (int step = 0; step < NSTEPS; step++) {
         if (collapse) {
             if (strcmp(schedule_type, "static") == 0) {
                 #pragma omp parallel for collapse(3) schedule(static, chunk_size)
                 for (int i = 1; i < N-1; i++)
                     for (int j = 1; j < N-1; j++)
                         for (int k = 1; k < N-1; k++) {
                             float laplacian = (
                                 u[i+1][j][k] + u[i-1][j][k] +
                                 u[i][j+1][k] + u[i][j-1][k] +
                                 u[i][j][k+1] + u[i][j][k-1] -
                                 6.0f * u[i][j][k]) / (DX * DX);
                             u_new[i][j][k] = u[i][j][k] + DT * VISC * laplacian;
                         }
             } else if (strcmp(schedule_type, "dynamic") == 0) {
                 #pragma omp parallel for collapse(3) schedule(dynamic, chunk_size)
                 for (int i = 1; i < N-1; i++)
                     for (int j = 1; j < N-1; j++)
                         for (int k = 1; k < N-1; k++) {
                             float laplacian = (
                                 u[i+1][j][k] + u[i-1][j][k] +
                                 u[i][j+1][k] + u[i][j-1][k] +
                                 u[i][j][k+1] + u[i][j][k-1] -
                                 6.0f * u[i][j][k]) / (DX * DX);
                             u_new[i][j][k] = u[i][j][k] + DT * VISC * laplacian;
                         }
             } else if (strcmp(schedule_type, "guided") == 0) {
                 #pragma omp parallel for collapse(3) schedule(guided, chunk_size)
                 for (int i = 1; i < N-1; i++)
                     for (int j = 1; j < N-1; j++)
                         for (int k = 1; k < N-1; k++) {
                             float laplacian = (
                                 u[i+1][j][k] + u[i-1][j][k] +
                                 u[i][j+1][k] + u[i][j-1][k] +
                                 u[i][j][k+1] + u[i][j][k-1] -
                                 6.0f * u[i][j][k]) / (DX * DX);
                             u_new[i][j][k] = u[i][j][k] + DT * VISC * laplacian;
                         }
 
                 if (step % SNAPSHOT_INTERVAL == 0 && chunk_size == 8 && collapse == 1 && strcmp(schedule_type, "guided") == 0) {
                     save_snapshot(bin_file);
                 }
             }
         } else {
             #pragma omp parallel for schedule(static, chunk_size)
             for (int i = 1; i < N-1; i++)
                 for (int j = 1; j < N-1; j++)
                     for (int k = 1; k < N-1; k++) {
                         float laplacian = (
                             u[i+1][j][k] + u[i-1][j][k] +
                             u[i][j+1][k] + u[i][j-1][k] +
                             u[i][j][k+1] + u[i][j][k-1] -
                             6.0f * u[i][j][k]) / (DX * DX);
                         u_new[i][j][k] = u[i][j][k] + DT * VISC * laplacian;
                     }
         }
 
         memcpy(u, u_new, sizeof(u));
     }
 
     double end = omp_get_wtime();
     double elapsed = end - start;
 
     printf("Config: %s, chunk=%d, collapse=%d -> %.3f s\n", schedule_type, chunk_size, collapse, elapsed);
     fprintf(log_file, "%s,%d,%d,%.6f\n", schedule_type, chunk_size, collapse, elapsed);
 }
 
 /**
  * @brief Entry point: runs simulations with various OpenMP configurations.
  */
 int main() {
     FILE *f = fopen("./task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks.csv", "w");
     if (!f) {
         perror("Failed to open benchmark CSV file.");
         return 1;
     }
 
     FILE *bin_file = fopen("./task-11.impact-of-schedule-and-collapse-clauses/data/fluid_with_perturbation.bin", "wb");
     if (!bin_file) {
         perror("Failed to open binary output file.");
         return 1;
     }
 
     save_csv_header(f);
 
     const char *schedules[] = {"static", "dynamic", "guided"};
     int chunk_sizes[] = {1, 2, 4, 8};
     int collapses[] = {0, 1};
 
     for (int s = 0; s < 3; s++) {
         for (int c = 0; c < 4; c++) {
             for (int col = 0; col < 2; col++) {
                 run_simulation(schedules[s], chunk_sizes[c], collapses[col], f, bin_file);
             }
         }
     }
 
     fclose(f);
     fclose(bin_file);
 
     printf("All tests completed. Results saved to './task-11.impact-of-schedule-and-collapse-clauses/data/benchmarks.csv'.\n");
     return 0;
 }
 