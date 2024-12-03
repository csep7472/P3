//
// Created by david on 12/2/2024.
//

#ifndef WORDLEPROJECT_HASHMAP_H
#define WORDLEPROJECT_HASHMAP_H

#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <utility>
#include <stdexcept>
#include <algorithm>

template <typename K, typename V>
class HashMap {
private:
    // Pair to store key-value
    using Pair = std::pair<K, V>;

    // Bucket (list of pairs)
    using Bucket = std::list<Pair>;

    // Vector of buckets
    std::vector<Bucket> table;

    // Number of elements
    size_t numElements;

    // Hash function
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % table.size();
    }

    // Find a key in a bucket
    typename Bucket::iterator findInBucket(Bucket& bucket, const K& key) {
        return std::find_if(bucket.begin(), bucket.end(),
                            [&key](const Pair& p) { return p.first == key; });
    }

public:
    // Constructor
    explicit HashMap(size_t size = 16) : table(size), numElements(0) {}

    // Overload [] for insertion and access
    V& operator[](const K& key) {
        size_t index = hash(key);
        Bucket& bucket = table[index];

        auto it = findInBucket(bucket, key);
        if (it == bucket.end()) {
            bucket.emplace_back(key, V{}); // Insert default value if key not found
            ++numElements;
            return bucket.back().second;
        } else {
            return it->second; // Return value if key found
        }
    }

    // Get value by key (const version)
    const V& operator[](const K& key) const {
        size_t index = hash(key);
        const Bucket& bucket = table[index];

        auto it = std::find_if(bucket.begin(), bucket.end(),
                               [&key](const Pair& p) { return p.first == key; });

        if (it == bucket.end()) {
            throw std::out_of_range("Key not found");
        } else {
            return it->second;
        }
    }

    // Get number of elements
    size_t size() const {
        return numElements;
    }

    // Clear the hashmap
    void clear() {
        for (auto& bucket : table) {
            bucket.clear();
        }
        numElements = 0;
    }
};

#endif // HASHMAP_H


#endif //WORDLEPROJECT_HASHMAP_H
