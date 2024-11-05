#!/bin/bash
#SBATCH --job-name=mpi_job
#SBATCH --output=exercicio4.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB 
#SBATCH --cpus-per-task=1
#SBATCH --time=00:05:00

mpirun ./exercicio4