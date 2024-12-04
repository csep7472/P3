#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <string>
#include <functional> // for std::hash

template <typename Key, typename Value>
class HashMap {
private:
    struct Entry {
        Key key;
        Value value;
    };

    std::vector<std::list<Entry>> table; // Hash table with chaining for collision handling
    size_t numBuckets;                   // Number of buckets in the hash table
    size_t numElements;                  // Number of elements currently in the hash map

    // Hash function to map keys to indices
    size_t getBucketIndex(const Key& key) const {
        return std::hash<Key>()(key) % numBuckets;
    }

public:
    // Default constructor
    HashMap(size_t initialCapacity = 16)
            : numBuckets(initialCapacity), numElements(0) {
        table.resize(numBuckets);
    }

    // Insert a key-value pair into the hash map
    void insert(const Key& key, const Value& value) {
        size_t bucketIndex = getBucketIndex(key);
        for (auto& entry : table[bucketIndex]) {
            if (entry.key == key) {
                entry.value = value; // Update existing value
                return;
            }
        }
        table[bucketIndex].push_back({key, value});
        numElements++;
    }

    // Retrieve a value by key
    Value& operator[](const Key& key) {
        size_t bucketIndex = getBucketIndex(key);
        for (auto& entry : table[bucketIndex]) {
            if (entry.key == key) {
                return entry.value;
            }
        }
        // Insert default value if key is not found
        table[bucketIndex].push_back({key, Value()});
        numElements++;
        return table[bucketIndex].back().value;
    }

    // Check if a key exists in the hash map
    bool contains(const Key& key) const {
        size_t bucketIndex = getBucketIndex(key);
        for (const auto& entry : table[bucketIndex]) {
            if (entry.key == key) {
                return true;
            }
        }
        return false;
    }

    // Remove a key-value pair by key
    void erase(const Key& key) {
        size_t bucketIndex = getBucketIndex(key);
        auto& bucket = table[bucketIndex];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                numElements--;
                return;
            }
        }
    }

    // Get the number of elements in the hash map
    size_t size() const {
        return numElements;
    }

    // Get the number of buckets in the hash map
    size_t capacity() const {
        return numBuckets;
    }
};

#endif // HASHMAP_H
