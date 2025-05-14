#!/bin/bash
#SBATCH --job-name=ERNANE-TASK13
#SBATCH --output=results-task-13-%j.out
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=24
#SBATCH --time=1-00:00:00
#SBATCH --partition=intel-512

gcc -O3 -fopenmp -o navier main.c

EXEC=./navier

echo "Início dos testes: $(date)"
echo "Usando N=512 e NSTEPS=1000"
echo "Núcleos disponíveis: $SLURM_CPUS_ON_NODE"
echo ""

AFFINITIES=("false" "true" "close" "spread" "master")
THREADS=(1 2 4 8 12 16 20 24)

for affinity in "${AFFINITIES[@]}"; do
    echo "============================================="
    echo "Testando OMP_PROC_BIND=$affinity"
    echo "============================================="
    echo ""

    export OMP_PROC_BIND=$affinity
    export OMP_PLACES=cores

    for nth in "${THREADS[@]}"; do
        echo "--- Rodando com $nth threads ---"
        export OMP_NUM_THREADS=$nth
        $EXEC
        echo ""
    done

    echo ""
done

echo "Fim dos testes: $(date)"