#include "../src/config.hpp"
#include <sstream>


namespace config {


const char split_separator = ' ';


void Map(const std::string& data, MapperContext<>& context) {
    std::stringstream stream(data);
    std::string word;

    while (stream >> word) {
        context.addPair(word, "1");
    }
}


std::pair<std::string, std::string> Reduce(const std::string& key, const std::list<std::reference_wrapper<std::string>>& values) {
    return std::make_pair(key, std::to_string(values.size()));
}


}
