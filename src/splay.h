#ifndef SPLAY_H
#define SPLAY_H
#include <string>
#include <iostream>
#include "api.h"

using namespace std;

struct Node{
    Game game;
    Node* left;
    Node* right;

    Node (Game game){
        this->game = game;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class SplayTree{
private:
    Node* root = nullptr;

    // Helper function to insert a new node
    Node* insertHelper(Node* node, Game game){
        if (node == nullptr){
            cout << "successful insert" << endl;
            return new Node(game);
        }
    }

    Node* rotateLeft(Node *node) {
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
        // no nodes, or game already at the top of tree, return immediately
        if (root == nullptr || root->game.name == game.name) {
            return root;
        }

        // Game is left of current root
        if (game.name < root->game.name) {
            if (root->left == nullptr) {
                return root;
            }

            // left left case
            if (game.name < root->left->game.name) {
                root->left->left = splay(root->left->left, game);
                root = rotateRight(root);
            }

            // left right case
            else if (game.name > root->right->game.name) {
                root->left->right = splay(root->left->right, game);
                if (root->left->right != nullptr) {
                    root->left = rotateLeft(root->left);
                }
            }

            if (root->left == nullptr) {
                return root;
            }
            else {
                return rotateRight(root);
            }
        }

        else if (game.name > root->game.name){
            if (root->right == nullptr) {
                return root;
            }

            // Right-Right (Zig-Zig) case
            if (game.name > root->right->game.name) {
                root->right->right = splay(root->right->right, game);
                root = rotateLeft(root);
            }
            // Right-Left (Zig-Zag) case
            else if (game.name < root->right->game.name) {
                root->right->left = splay(root->right->left, game);
                if (root->right->left != nullptr) {
                    root->right = rotateRight(root->right);
                }
            }

            if (root->left == nullptr) {
                return root;
            }
            else {
                return rotateLeft(root);
            }
        }
    }

public:
    void insert(Game game) {
        if (root == nullptr) {
            root = new Node(game);
            cout << "successfully insert" << endl;
            return;
        }

        root = splay(root, game);

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
        cout << "successfully insert" << endl;
    }
};
#endif //SPLAY_H
