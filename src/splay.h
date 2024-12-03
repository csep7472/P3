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

    Node* insertHelper(Node* node, Game game) {
        if (node == nullptr) {
            return new Node(game);
        }

        if (game.name < node->game.name) {
            node->left = insertHelper(node->left, game);
        } else {
            node->right = insertHelper(node->right, game);
        }

        return node;
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
        if (root == nullptr || root->game.name == game.name) {
            return root;
        }


        if (game.name < root->game.name) {

            if (root->left == nullptr) {
                return root;
            }


            if (game.name < root->left->game.name) {
                root->left = splay(root->left, game);
                root = rotateRight(root);
            }

            else if (game.name > root->left->game.name) {
                root->left->right = splay(root->left->right, game);
                if (root->left->right != nullptr) {
                    root->left = rotateLeft(root->left);
                }
            }

            return (root->left == nullptr) ? root : rotateRight(root);
        }
        else {

            if (root->right == nullptr) {
                return root;
            }


            if (game.name > root->right->game.name) {
                root->right = splay(root->right, game);
                root = rotateLeft(root);
            }

            else if (game.name < root->right->game.name) {
                root->right->left = splay(root->right->left, game);
                if (root->right->left != nullptr) {
                    root->right = rotateRight(root->right);
                }
            }

            return (root->right == nullptr) ? root : rotateLeft(root);
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

        if (root->game.name == game.name) {
            cout << "game already exists" << endl;
            return;
        }

        // Create a new node for the game
        Node* newNode = new Node(game);

        // Insert the new node in the tree
        if (game.name < root->game.name) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        } else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }

        // The new node becomes the root of the tree
        root = newNode;
        cout << "successfully insert" << endl;
    }
};
#endif //SPLAY_H
