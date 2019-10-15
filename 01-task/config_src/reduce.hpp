#include <string>
#include <list>


std::pair<std::string, std::string> Reduce(const std::string& key, const std::list<std::reference_wrapper<std::string>>& values) {
    return std::make_pair(key, std::to_string(values.size()));
}
