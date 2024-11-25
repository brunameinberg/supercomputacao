#!/bin/bash
#SBATCH --job-name=prova_job
#SBATCH --partition=prova
#SBATCH --ntasks=2
#SBATCH --mem=3G
#SBATCH --cpus-per-task=5
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=10

./executavel