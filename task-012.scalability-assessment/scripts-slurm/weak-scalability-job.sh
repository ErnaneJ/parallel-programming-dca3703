#!/bin/bash
#SBATCH --job-name=ERNANE-OPENMP-TASK12-WEAK
#SBATCH --time=0-0:20
#SBATCH --partition=intel-512
#SBATCH --output=slurm-weak-scalability-%j.out

echo "Inicializando JOB!"

baseN=32  # N base para 1 thread

for threads in 1 2 4 8 16 24
do
  export OMP_NUM_THREADS=$threads

  # Escala o volume total de trabalho proporcional ao número de threads (weak scaling)
  # O volume é mantido constante por thread: N³ ~ constante * threads
  N=$(awk "BEGIN { print int($baseN * ($threads ^ (1.0/3.0))) }")

  if (( N < 3 )); then
    echo "N calculado muito pequeno ($N). Ajustando para N=3"
    N=3
  fi

  echo "Executando com $OMP_NUM_THREADS threads e N=$N"

  /home/efrjunior/programacao_paralela/task-012.scalability-assessment/weak-scalability-assessment $N
done

echo "Finalizando JOB!"