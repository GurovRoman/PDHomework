#!/bin/sh


module load mpi/openmpi4-x86_64
module load gcc/9.2.0

mpic++ -std=c++17 -fopenmp -lrt -g src/main.cpp src/mapper_routine.cpp src/master_routine.cpp src/reducer_routine.cpp config_src/config.cpp -o main
