#!/bin/bash
#SBATCH --job-name=pi_montecarlo
#SBATCH --output=piparalelov1.txt
#SBATCH --ntasks=1
#SBATCH --mem=1024MB 
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

./piparalelov1