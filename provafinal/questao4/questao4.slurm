#!/bin/bash
#SBATCH --job-name=questao4
#SBATCH --output=questao4.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB
#SBATCH --cpus-per-task=5
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=4

mpirun ./questao4
