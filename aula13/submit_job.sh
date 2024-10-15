#!/bin/bash
#SBATCH --job-name=omp_scheduler_test
#SBATCH --output=vector_insert_omp_parallel3.txt
#SBATCH --ntasks=1
#SBATCH --mem=1024MB 
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

./vector_insert_omp_parallel