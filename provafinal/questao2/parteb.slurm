#!/bin/bash
#SBATCH --job-name=parteb_job
#SBATCH --output=questao2parteb.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

export OMP_NUM_THREADS=4

./parteb
