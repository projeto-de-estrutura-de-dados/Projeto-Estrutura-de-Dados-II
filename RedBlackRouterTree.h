#ifndef REDBLACKROUTERTREE_H
#define REDBLACKROUTERTREE_H

#include "PacketRule.h"
using namespace std;

class RedBlackRouterTree {

private:

    enum Color {
        RED,
        BLACK
    };

    struct Node {
        PacketRule rule;
        Color color;

        Node* left;
        Node* right;
        Node* parent;

        Node(PacketRule r) :
            rule(r),
            color(RED),
            left(nullptr),
            right(nullptr),
            parent(nullptr) {}
    };

    Node* root = nullptr;
    int rotations = 0;

    // ROTACAO PARA ESQUERDA
    Node* rotateLeft(Node* x) {

        rotations++;

        Node* y = x->right;

        x->right = y->left;

        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;

        return y;
    }

    // ROTACAO PARA DIREITA
    Node* rotateRight(Node* y) {

        rotations++;

        Node* x = y->left;

        y->left = x->right;

        if (x->right != nullptr)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == nullptr)
            root = x;
        else if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;

        x->right = y;
        y->parent = x;

        return x;
    }

    // INSERT SIMPLES
    Node* insert(Node* root, Node* node) {

        if (root == nullptr)
            return node;

        if (node->rule.id < root->rule.id) {

            root->left = insert(root->left, node);
            root->left->parent = root;

        } else if (node->rule.id > root->rule.id) {

            root->right = insert(root->right, node);
            root->right->parent = root;
        }

        return root;
    }

    // BALANCEAMENTO APOS INSERT
    void fixInsert(Node* node) {

        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while (node != root &&
               node->color == RED &&
               node->parent->color == RED) {

            parent = node->parent;
            grandparent = parent->parent;

            // PAI ESTA NA ESQUERDA
            if (parent == grandparent->left) {

                Node* uncle = grandparent->right;

                // CASO 1 -> TIO VERMELHO
                if (uncle != nullptr && uncle->color == RED) {

                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;

                    node = grandparent;
                }
                else {

                    // CASO 2 -> ESQUERDA-DIREITA
                    if (node == parent->right) {

                        rotateLeft(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // CASO 3 -> ESQUERDA-ESQUERDA
                    rotateRight(grandparent);

                    Color temp = parent->color;
                    parent->color = grandparent->color;
                    grandparent->color = temp;

                    node = parent;
                }
            }

            // PAI ESTA NA DIREITA
            else {

                Node* uncle = grandparent->left;

                // CASO 1 -> TIO VERMELHO
                if (uncle != nullptr && uncle->color == RED) {

                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;

                    node = grandparent;
                }
                else {

                    // CASO 2 -> DIREITA-ESQUERDA
                    if (node == parent->left) {

                        rotateRight(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // CASO 3 -> DIREITA-DIREITA
                    rotateLeft(grandparent);

                    Color temp = parent->color;
                    parent->color = grandparent->color;
                    grandparent->color = temp;

                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

public:

    // INSERT PUBLICO
    void insert(PacketRule rule) {

        Node* node = new Node(rule);

        root = insert(root, node);

        fixInsert(node);
    }

    int getRotations() {
        return rotations;
    }
};

#endif