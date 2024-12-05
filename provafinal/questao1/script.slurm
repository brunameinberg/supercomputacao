#!/bin/bash
#SBATCH --job-name=prova_job
#SBATCH --partition=prova
#SBATCH --ntasks=5
#SBATCH --mem=16G
#SBATCH --cpus-per-task=4
#SBATCH --time=01:00:00

export OMP_NUM_THREADS=15

./programa