#!/bin/bash
#SBATCH --job-name=prova_job
#SBATCH --output=questao3paralelizada1500N_8cores.txt
#SBATCH --ntasks=4
#SBATCH --mem=1024MB
#SBATCH --cpus-per-task=8
#SBATCH --time=00:05:00


./questao3paralelizada