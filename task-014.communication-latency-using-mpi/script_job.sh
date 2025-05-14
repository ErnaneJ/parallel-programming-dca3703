#!/bin/bash
#SBATCH --job-name=PINGPONG
#SBATCH --output=results-pingpong-%j.out
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1
#SBATCH --time=01:00:00
#SBATCH --partition=intel-512

# Compila o programa
mpicc -O3 -o pingpong main.c

# Define o executável
EXEC=./pingpong

echo "============================================="
echo "Início dos testes MPI PingPong: $(date)"
echo "Executável: $EXEC"
echo "============================================="

export OMPI_MCA_btl="self,vader" # Remove warnings sobre o driver OpenFabrics/openib

# Executa com 2 tarefas MPI
mpirun -np 2 $EXEC

echo "============================================="
echo "Fim dos testes: $(date)"