//
// Created by david on 11/27/2024.
//
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

#ifndef WORDLEPROJECT_TRIE_H
#define WORDLEPROJECT_TRIE_H

struct TrieNode {
    bool isEndOfWord;
    unordered_map<char, TrieNode*> children;
    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    TrieNode* root;

    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c])
                node->children[c] = new TrieNode();
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    bool search(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c])
                return false;
            node = node->children[c];
        }
        return node->isEndOfWord;
    }
};


#endif //WORDLEPROJECT_TRIE_H
