#!/bin/sh

module load mpi/openmpi4-x86_64

mpiexec ./main $1 $2 2> /dev/null