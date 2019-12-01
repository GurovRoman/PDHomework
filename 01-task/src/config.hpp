#include "mapper_context.hpp"
#include <string>
#include <list>


namespace config {


extern const char split_separator;

void Map(const std::string& data, MapperContext<>& context);

std::pair<std::string, std::string> Reduce(const std::string& key, const std::list<std::reference_wrapper<std::string>>& values);


}
