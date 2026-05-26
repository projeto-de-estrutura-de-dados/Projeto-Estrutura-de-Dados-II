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

public:

    int getRotations() {
        return rotations;
    }
};

#endif