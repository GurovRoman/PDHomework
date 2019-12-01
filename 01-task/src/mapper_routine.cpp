#include <string>
#include <fstream>
#include <mpi.h>
#include "mapper_context.hpp"
#include "config.hpp"


std::string getSplit(std::istream& input, size_t start_pos, size_t size) {
    size_t split_start;

    if (start_pos == 0) {
        split_start = start_pos;
    } else {
        input.seekg(start_pos);
        
        input.ignore(size, config::split_separator);

        split_start = input.gcount();

        if (split_start == size) {
            return std::string();
        }

        split_start += start_pos;
    }

    size_t split_end = start_pos + size;

    input.seekg(split_end);
    
    input.ignore(std::numeric_limits<std::streamsize>::max(), config::split_separator);

    split_end += input.gcount();

    std::string split;
    split.resize(split_end - split_start);

    input.clear();
    input.seekg(split_start);
    input.read(split.data(), split_end - split_start);

    return split;
}


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

        std::ifstream file(pathname);
        std::string split = getSplit(file, start_pos, size);
        file.close();

        if (!split.empty()) {
            config::Map(split, context);
        }
    }

    auto data = context.ejectData();

    for (size_t i = 0; i < reducer_count; ++i) {
        std::ofstream file("reduce_" + std::to_string(i + 1) + '_' + std::to_string(rank));
        for (auto& pair : data[i]) {
            file << pair.first << '\t' << pair.second << '\n';
        }
    }
}
