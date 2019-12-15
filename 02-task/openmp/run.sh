#!/bin/sh
#SBATCH --partition=RT_study
#SBATCH --job-name=MapReduce_OMP
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=16
 
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


module load mpi/openmpi4-x86_64

mpiexec ./main $1 $2 2> /dev/null
