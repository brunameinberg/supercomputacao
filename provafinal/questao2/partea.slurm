#!/bin/bash
#SBATCH --job-name=partea_job
#SBATCH --output=questao2partea.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=4

./partea
