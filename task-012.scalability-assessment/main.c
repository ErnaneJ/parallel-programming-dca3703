/**
 * @file main.c
 * @brief Optimized 3D heat diffusion using best scheduling strategy from benchmark.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>
 #include <omp.h>
 
 #define NSTEPS 1000
 #define DT 0.01f
 #define DX 1.0f
 #define VISC 0.1f
 #define SNAPSHOT_INTERVAL 1000
 
 int N;
 float ***u, ***u_new;
 
 void allocate_grids() {
     u = malloc(N * sizeof(float **));
     u_new = malloc(N * sizeof(float **));
     for (int i = 0; i < N; i++) {
         u[i] = malloc(N * sizeof(float *));
         u_new[i] = malloc(N * sizeof(float *));
         for (int j = 0; j < N; j++) {
             u[i][j] = calloc(N, sizeof(float));
             u_new[i][j] = calloc(N, sizeof(float));
         }
     }
 }
 
 void free_grids() {
     for (int i = 0; i < N; i++) {
         for (int j = 0; j < N; j++) {
             free(u[i][j]);
             free(u_new[i][j]);
         }
         free(u[i]);
         free(u_new[i]);
     }
     free(u);
     free(u_new);
 }
 
 void initialize()
 {
     for (int i = 0; i < N; i++)
         for (int j = 0; j < N; j++)
             for (int k = 0; k < N; k++)
                 u[i][j][k] = 0.0f;
 
     int cx = N / 2, cy = N / 2, cz = N / 2;
     u[cx][cy][cz] = 1.0f;
 }
 
 void run_simulation()
 {
     allocate_grids();
     initialize();
     double start = omp_get_wtime();
 
 #pragma omp parallel for collapse(3) schedule(static, 1)
     for (int i = 1; i < N - 1; i++)
         for (int j = 1; j < N - 1; j++)
             for (int k = 1; k < N - 1; k++)
                 u_new[i][j][k] = 0.0f;
 
     for (int step = 0; step < NSTEPS; step++)
     {
 #pragma omp parallel for collapse(3) schedule(guided, 1024)
         for (int i = 1; i < N - 1; i++)
             for (int j = 1; j < N - 1; j++)
                 for (int k = 1; k < N - 1; k++)
                 {
                     float laplacian = (u[i + 1][j][k] + u[i - 1][j][k] +
                                        u[i][j + 1][k] + u[i][j - 1][k] +
                                        u[i][j][k + 1] + u[i][j][k - 1] -
                                        6.0f * u[i][j][k]) /
                                       (DX * DX);
                     u_new[i][j][k] = u[i][j][k] + DT * VISC * laplacian;
                 }
 
         #pragma omp parallel for collapse(3) schedule(static, 1)
         for (int i = 0; i < N; i++)
             for (int j = 0; j < N; j++)
                 for (int k = 0; k < N; k++)
                     u[i][j][k] = u_new[i][j][k];
     }
 
     double end = omp_get_wtime();
     printf("=> schedule(type=guided, chunk_size=1024) + collapse(3) -> %.3f s\n", end - start);
 
     free_grids();
 }
 
 int main(int argc, char *argv[])
 {
     if (argc != 2) {
         fprintf(stderr, "Use: %s <N>\n", argv[0]);
         return 1;
     }
 
     N = atoi(argv[1]);
     printf("Starting - Task 012 - Scalability assessment with N=%d\n", N);
     run_simulation();
     printf("Finished - Task 012 - Scalability assessment\n");
     return 0;
 }
 