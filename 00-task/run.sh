#!/bin/sh

module load mpi/openmpi4-x86_64

mpirun main ${1:-100} 2> /dev/null
