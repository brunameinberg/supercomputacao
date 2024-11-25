#!/bin/bash
#SBATCH --job-name=prova_job
#SBATCH --output=questao2.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB
#SBATCH --cpus-per-task=5
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=5

mpirun ./questao2