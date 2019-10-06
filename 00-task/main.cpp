#include <vector>
#include <string>
#include <numeric>
#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <ostream>


int getDataSizeForRank(int rank, int world_size, int data_size) {
    return data_size / world_size + (rank < data_size % world_size);
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int current_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int data_size = std::stoi(argv[1]);
    int chunk_size = getDataSizeForRank(current_rank, world_size, data_size);
    std::vector<int> data;

    if (current_rank == 0) {
        data.resize(data_size);
        std::iota(data.begin(), data.end(), 0);
        auto cur_data = data.data() + chunk_size;
        for (int i = 1; i < world_size; ++i) {
            auto size = getDataSizeForRank(i, world_size, data_size);
            MPI_Send(cur_data, size, MPI_INT, i, 0, MPI_COMM_WORLD);
            cur_data += size;
        }
    } else {
        data.resize(chunk_size);
        MPI_Recv(data.data(), chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, nullptr);
    }

    long long sum = std::accumulate(data.begin(), data.begin() + chunk_size, 0);

    if (current_rank != 0) {
        MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        std::vector<long long> partial_sums(world_size);

        partial_sums[0] = sum;
        for (int i = 1; i < world_size; ++i) {
            MPI_Recv(partial_sums.data() + i, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, nullptr);
        }
        
        auto final_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0);

        std::cout << "Partial sums from nodes:\n";
        for (size_t i = 0; i < partial_sums.size(); ++i) {
            std::cout << "rank: " << i << ", sum: " << partial_sums[i] << '\n';
        }
        long long manual_sum = std::accumulate(data.begin(), data.end(), 0);
        std::cout << "\nFinal sum is: " << final_sum << "\nShould be:    " << manual_sum << '\n';
        std::cout << (final_sum == manual_sum ? "Everything is fine" : "Assertion failed") << '\n';
    }

    MPI_Finalize();
}
