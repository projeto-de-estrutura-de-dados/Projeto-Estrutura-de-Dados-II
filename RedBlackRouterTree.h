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

public:

    int getRotations() {
        return rotations;
    }
};

#endif