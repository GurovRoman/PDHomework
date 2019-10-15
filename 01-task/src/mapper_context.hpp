#pragma once
#include <string>
#include <vector>
#include <algorithm>


template<class Key = std::string, class Value = std::string>
class MapperContext {
    public:
        MapperContext(size_t size) : bucket_count(size), buckets(size) {}

        void addPair(const Key& key, const Value& value) {
            auto hash = std::hash<Key>()(key) % bucket_count;
            buckets[hash].emplace_back(key, value);        
        }
    
        auto ejectData() {
            for (auto& bucket : buckets) {
                std::sort(bucket.begin(), bucket.end());
            }
            return std::move(buckets);
        }

    private:
        size_t bucket_count;
        std::vector<std::vector<std::pair<Key, Value>>> buckets;
};