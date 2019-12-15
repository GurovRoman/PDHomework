#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <mpi.h>
#include <omp.h>
#include "routines.hpp"
#include "config.hpp"
#include <filesystem>

namespace fs = std::filesystem;


void reducer_routine(int rank, int world_size) {
    std::vector<std::pair<std::string, std::string>> pairs;

    for (int i = 1; i < world_size; ++i) {
        for (int j = 0; j < omp_get_max_threads(); ++j) {
            std::string filename = "reduce_" + std::to_string(rank) + '_' + std::to_string(i) + '_' + std::to_string(j);
            std::ifstream file(filename);
            while (file) {
                std::string key, value;
                if (std::getline(file, key, '\t') && std::getline(file, value, '\n')) {
                    pairs.emplace_back(key, value);
                }
            }
            file.close();
            fs::remove(filename);
        }
    }

    std::sort(pairs.begin(), pairs.end());

    for (auto& pair: pairs) {
        
    }


    #pragma omp parallel default(shared)
    {
        std::list<std::reference_wrapper<std::string>> values;
        std::list<std::string> result;
        
        size_t chunk_size = pairs.size() / omp_get_num_threads();

        #pragma omp for schedule(static, chunk_size)
        for (size_t i = 0; i < pairs.size(); ++i) {
            values.emplace_back(pairs[i].second);
            if (i == pairs.size() - 1 || pairs[i + 1].first != pairs[i].first) {
                auto res = config::Reduce(pairs[i].first, values);
                result.emplace_back(res.first + '\t' + res.second + '\n');
                values.clear();
            }
        }

        #pragma omp critical
        {
            std::ofstream output("result_" + std::to_string(rank));
            for (auto& res : result) {
                output << res;
            }
        }
    }
}