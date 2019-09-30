#include <vector>
#include <string>
#include <numeric>
#include <iostream>
#include <mpi.h>
#include <algorithm>
#include <ostream>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int current_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int data_size = std::stoi(argv[1]);
    std::vector<int> data;

    if (current_rank == 0) {
        data.resize(data_size, 0);
        std::iota(data.begin(), data.end(), 0);
    }

    if (data_size % world_size) {
        data_size += world_size - data_size % world_size;
    }

    data.resize(data_size, 0);

    std::vector<int> chunk(data_size / world_size);
    MPI_Scatter(data.data(), chunk.size(), MPI_INT, chunk.data(), chunk.size(), MPI_INT, 0, MPI_COMM_WORLD);

    long long sum = std::accumulate(chunk.begin(), chunk.end(), 0);

    long long final_sum = 0;
    MPI_Reduce(&sum, &final_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    std::vector<long long> partial_sums(world_size);
    MPI_Gather(&sum, 1, MPI_LONG_LONG, partial_sums.data(), 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    if (current_rank == 0) {
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
