#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <mpi.h>
#include "routines.hpp"
#include "config.hpp"
#include <filesystem>

namespace fs = std::filesystem;


void reducer_routine(int rank, int world_size) {
    std::vector<std::pair<std::string, std::string>> pairs;

    for (int i = 1; i < world_size; ++i) {
        std::string filename = "reduce_" + std::to_string(rank) + '_' + std::to_string(i);
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

    std::sort(pairs.begin(), pairs.end());

    std::ofstream output("result_" + std::to_string(rank));
    std::list<std::reference_wrapper<std::string>> values;

    for (auto it = pairs.begin(); it != pairs.end(); ++it) {
        values.emplace_back(it->second);
        if (it == pairs.end() - 1 || (it + 1)->first != it->first) {
            auto res = config::Reduce(it->first, values);
            output << res.first << '\t' << res.second << '\n';
            values.clear();
        }
    }

}