#!/bin/sh

module load mpi/openmpi4-x86_64

mpic++ -std=c++11 -g main.cpp -o main
