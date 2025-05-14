#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

#define N 256
#define NSTEPS 1000
#define DT 0.01f
#define DX 1.0f
#define VISC 0.1f
#define SNAPSHOT_INTERVAL 1000 // opcional, caso queira salvar

float u[N][N][N], u_new[N][N][N];

void initialize()
{
  memset(u, 0, sizeof(u));
  int cx = N / 2, cy = N / 2, cz = N / 2;
  u[cx][cy][cz] = 1.0f;
}

void save_snapshot(FILE *f)
{
  fwrite(u, sizeof(float), N * N * N, f);
}

void run_simulation()
{
  initialize();
  double start = omp_get_wtime();

#pragma omp parallel
  {
#pragma omp for collapse(3) schedule(static, 1)
    for (int i = 1; i < N - 1; i++)
      for (int j = 1; j < N - 1; j++)
        for (int k = 1; k < N - 1; k++)
          u_new[i][j][k] = 0.0f;
  }

  for (int step = 0; step < NSTEPS; step++)
  {
#pragma omp parallel
    {
#pragma omp for collapse(3) schedule(guided, 1024)
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
    }

    memcpy(u, u_new, sizeof(u));
  }

  double end = omp_get_wtime();
  printf("=> %.3f s\n", end - start);
}

int main()
{
  run_simulation();
  return 0;
}