#!/bin/bash
#SBATCH --job-name=ERNANE-TASK015-HEATDIFF
#SBATCH --output=results-heatdiff-scaling-%j.out
#SBATCH --ntasks=32
#SBATCH --cpus-per-task=1
#SBATCH --time=1-00:00:00
#SBATCH --partition=intel-512

# Compila uma única vez
mpicc -O3 -o heat_diffusion main.c

EXEC=./heat_diffusion

# Evita warnings de OpenMPI sobre interconexão
export OMPI_MCA_btl="self,vader"

echo "============================================="
echo "Início da simulação MPI de difusão de calor: $(date)"
echo "Executável: $EXEC"
echo "============================================="

# Testa com múltiplos valores de processos
for P in 2 4 8 16 32; do
  echo "---------------------------------------------"
  echo "Testando com $P processos MPI..."
  echo "---------------------------------------------"
  mpirun -np $P $EXEC
  echo ""
done

echo "============================================="
echo "Fim da simulação: $(date)"