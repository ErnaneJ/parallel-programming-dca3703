#!/bin/bash
#SBATCH --job-name=ERNANE-TASK16-MATVEC
#SBATCH --output=results-matvec-scaling-%j.out
#SBATCH --ntasks=32
#SBATCH --cpus-per-task=1
#SBATCH --time=1-00:00:00
#SBATCH --partition=intel-512

# Compila o código C
mpicc -O3 -o matvec main.c

EXEC=./matvec

# Evita warnings de OpenMPI sobre interconexão
export OMPI_MCA_btl="self,vader"

echo "============================================="
echo "Início da simulação MPI de multiplicação A*x com matrizes quadradas: $(date)"
echo "Executável: $EXEC"
echo "============================================="

# Testa com diferentes números de processos e tamanhos de matrizes quadradas
for P in 2 4 8 16 32; do
  for SIZE in 512 1024 2048 3072 4096; do
    for REP in 1 2 3; do
      echo "---------------------------------------------"
      echo "Testando com $P processos MPI e matriz $SIZE x $SIZE (execução $REP)"
      echo "---------------------------------------------"
      mpirun -np $P $EXEC $SIZE $SIZE
      echo ""
    done
  done
done

echo "============================================="
echo "Fim da simulação: $(date)"
