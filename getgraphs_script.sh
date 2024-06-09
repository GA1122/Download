#!/bin/sh
#SBATCH --job-name=getgraphs_script
#SBATCH --time=72:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --partition=defq
#SBATCH --gres=gpu:1
#SBATCH --err="/home/gas690/Download/code_gnn/sastvd/scripts/hpc/logs/getgraphs_%A_%a.out"
#SBATCH --output="/home/gas690/Download/code_gnn/sastvd/scripts/hpc/logs/getgraphs_%A_%a.out"

## in the list above, the partition name depends on where you are running your job.
## On DAS5 the default would be `defq` on Lisa the default would be `gpu` or `gpu_shared`
## Typing `sinfo` on the server command line gives a column called PARTITION. There, one can find the
##name of a specific node, the state (down, alloc, idle etc), the availability and how long is the time
##limit . Ask your supervisor before running jobs on queues you do not know.



# Load GPU drivers

## Enable the following two lines for DAS5
# module load cuda12.1/toolkit
# module load cuDNN/cuda12.1/9.1.0.70

## Enable the following line for DAS6
module load cuda11.7/toolkit

## For Lisa and Snellius, modules are usually not needed
## https://userinfo.surfsara.nl/systems/shared/modules

module load java/jdk-19

# This loads the anaconda virtual environment with our packages
source $HOME/.bashrc
conda activate

# Run the actual experiment.
python -u /home/gas690/Download/code_gnn/sastvd/scripts/getgraphs.py bigvul --num_jobs 100 --overwrite $@
