#!/bin/bash
#SBATCH --job-name=ERNANE-OPENMP-TASK12
#SBATCH --time=0-0:20
#SBATCH --partition=intel-512
#SBATCH --output=slurm-strong-scalability-%j.out

echo "Inicializando JOB!"
for threads in 1 2 4 8 16 24
do
  export OMP_NUM_THREADS=$threads

  echo "Executando com $OMP_NUM_THREADS threads"

  /home/efrjunior/programacao_paralela/task-012.scalability-assessment/scalability-assessment
done

echo "Finalizando JOB!"