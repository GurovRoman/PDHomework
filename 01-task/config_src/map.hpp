#include "../src/mapper_context.hpp"
#include <sstream>


void Map(const std::string& data, MapperContext<>& context) {
    std::stringstream stream(data);
    std::string word;

    while (stream >> word) {
        context.addPair(word, "1");
    }
}