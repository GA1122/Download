#!/bin/sh
#SBATCH --job-name=replication_script
#SBATCH --time=72:00:00
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --partition=defq
#SBATCH --gres=gpu:1

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



# This loads the anaconda virtual environment with our packages
source $HOME/.bashrc
conda activate

# Base directory for the experiment
mkdir $HOME/experiments
cd $HOME/experiments

# Simple trick to create a unique directory for each run of the script
echo $$
mkdir o`echo $$`
cd o`echo $$`

# Run the actual experiment.
python /home/gas690/IDs.py
