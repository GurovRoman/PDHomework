#include <string>
#include <fstream>
#include <filesystem>
#include <list>
#include <mpi.h>

namespace fs = std::filesystem;


void master_routine(int world_size, const std::string& input, size_t block_size = 65535) {
    std::list<fs::path> files;
    {
        fs::path input_path(input);
        if (fs::is_regular_file(input_path)) {
            files.push_back(input_path);
        } else if (fs::is_directory(input_path)) {
            for (auto& entry : fs::directory_iterator(input_path)) {
                files.push_back(entry.path());
            }
        }
    }

    int cur_mapper = 0;
    for (auto& file_path : files) {
        auto size = fs::file_size(file_path);
        for (size_t start_pos = 0; start_pos < size; start_pos += block_size) {
            auto path = file_path.string();
            auto path_size = path.size();
            MPI_Send(&path_size, 1, MPI_UINT64_T, cur_mapper + 1, 0, MPI_COMM_WORLD);
            MPI_Send(path.c_str(), path_size, MPI_CHAR, cur_mapper + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&start_pos, 1, MPI_UINT64_T, cur_mapper + 1, 0, MPI_COMM_WORLD);
            auto clamped_block_size = std::min(block_size, size - start_pos);
            MPI_Send(&clamped_block_size, 1, MPI_UINT64_T, cur_mapper + 1, 0, MPI_COMM_WORLD);
            cur_mapper += 1;
            cur_mapper %= world_size - 1;
        }
    }

    for (int i = 1; i < world_size; ++i) {
        const size_t val = 0;
        MPI_Send(&val, 1, MPI_UINT64_T, i, 0, MPI_COMM_WORLD);
    }
}