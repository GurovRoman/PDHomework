#include <vector>
#include <string>
#include <numeric>
#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <ostream>

#include "routines.hpp"


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int current_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int reducer_count = std::stoi(argv[1]);
    std::string input = argv[2];

    if (current_rank == 0) {
        master_routine(world_size, input);
        
        MPI_Barrier(MPI_COMM_WORLD);
    } else {
        mapper_routine(current_rank, reducer_count);

        MPI_Barrier(MPI_COMM_WORLD);

        if (current_rank <= reducer_count) {
            reducer_routine(current_rank, world_size);
        }
    }

    MPI_Finalize();
}
