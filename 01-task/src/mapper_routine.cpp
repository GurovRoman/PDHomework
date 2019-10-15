#include <string>
#include <fstream>
#include <mpi.h>
#include "mapper_context.hpp"
#include "../config_src/map.hpp"


void mapper_routine(int rank, size_t reducer_count) {
    MapperContext context(reducer_count);

    while (true) {
        size_t path_size;
        MPI_Recv(&path_size, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, nullptr);
        
        if (!path_size) {
            break;
        }

        std::string pathname;
        pathname.resize(path_size);
        MPI_Recv(pathname.data(), path_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, nullptr);
        size_t start_pos, size;
        MPI_Recv(&start_pos, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, nullptr);
        MPI_Recv(&size, 1, MPI_UINT64_T, 0, 0, MPI_COMM_WORLD, nullptr);

        std::string block;
        block.resize(size);

        std::ifstream file(pathname);
        file.seekg(start_pos);
        file.read(block.data(), size);
        file.close();

        Map(block, context);
    }

    auto data = context.ejectData();

    for (size_t i = 0; i < reducer_count; ++i) {
        std::ofstream file("reduce_" + std::to_string(i + 1) + '_' + std::to_string(rank));
        for (auto& pair : data[i]) {
            file << pair.first << '\t' << pair.second << '\n';
        }
    }
}