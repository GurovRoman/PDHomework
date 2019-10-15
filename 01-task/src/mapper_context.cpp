#include "mapper_context.hpp"
#include <algorithm>


template<class Key, class Value>
void MapperContext<Key, Value>::addPair(const Key& key, const Value& value) {
    auto hash = std::hash<Key>()(key) % bucket_count;
    buckets[hash].emplace_back(key, value);
}

template<class Key, class Value>
auto MapperContext<Key, Value>::ejectData() {
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
    }
    return std::move(buckets);
}


template class MapperContext<>;