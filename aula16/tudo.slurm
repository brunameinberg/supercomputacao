#!/bin/bash
#SBATCH --job-name=mpi_job
#SBATCH --output=mpi_job_ex2.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB 
#SBATCH --cpus-per-task=1
#SBATCH --time=00:05:00

mpirun ./exercicio2