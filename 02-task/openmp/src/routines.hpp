#pragma once

#include <string>

void mapper_routine(int rank, size_t reducer_count);

void reducer_routine(int rank, int world_size);

void master_routine(int world_size, const std::string& input, size_t block_size = 65535);
