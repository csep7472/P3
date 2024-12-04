#ifndef SPLAY_H
#define SPLAY_H
#include <string>
#include <iostream>
#include "api.h"

using namespace std;

struct Node {
    Game game;
    Node* left;
    Node* right;

    Node(Game game) {
        this->game = game;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class SplayTree {
private:
    Node* root = nullptr;

    Node* rotateLeft(Node* node) {
        Node* newParent = node->right;
        node->right = newParent->left;
        newParent->left = node;
        return newParent;
    }

    Node* rotateRight(Node* node) {
        Node* newParent = node->left;
        node->left = newParent->right;
        newParent->right = node;
        return newParent;
    }

    Node* splay(Node* root, Game game) {
        if (root == nullptr || root->game.name == game.name) {
            return root;
        }

        if (game.name < root->game.name) {
            if (root->left == nullptr) return root;

            if (game.name < root->left->game.name) {
                root->left->left = splay(root->left->left, game);
                root = rotateRight(root);
            } else if (game.name > root->left->game.name) {
                root->left->right = splay(root->left->right, game);
                if (root->left->right != nullptr) {
                    root->left = rotateLeft(root->left);
                }
            }

            return (root->left == nullptr) ? root : rotateRight(root);
        } else {
            if (root->right == nullptr) return root;

            if (game.name > root->right->game.name) {
                root->right->right = splay(root->right->right, game);
                root = rotateLeft(root);
            } else if (game.name < root->right->game.name) {
                root->right->left = splay(root->right->left, game);
                if (root->right->left != nullptr) {
                    root->right = rotateRight(root->right);
                }
            }

            return (root->right == nullptr) ? root : rotateLeft(root);
        }
    }

    Game* searchHelper(Node* node, const string& name) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->game.name == name) {
            return &node->game;
        }

        if (name < node->game.name) {
            return searchHelper(node->left, name);
        }
        else {
            return searchHelper(node->right, name);
        }
    }

public:
    void insert(Game game) {
        if (root == nullptr) {
            root = new Node(game);
            return;
        }

        root = splay(root, game);

        if (game.name == root->game.name) {
            return;
        }

        Node* newNode = new Node(game);

        if (game.name < root->game.name) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }

        root = newNode;
    }

    Game* search(const string& name) {
        if (root == nullptr) {
            return nullptr;
        }

        Game game;
        game.name = name;

        root = splay(root, game);

        if (root->game.name == name) {
            return &root->game;
        }

        return nullptr;
    }
};
#endif //SPLAY_H