#ifndef AVLROUTERTREE_H
#define AVLROUTERTREE_H

#include "PacketRule.h"
#include <algorithm>
using namespace std;

class AVLRouterTree {

private:
    struct Node {
        PacketRule rule;
        Node* left;
        Node* right;
        int height;

        Node(PacketRule r) : rule(r), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root = nullptr;
    int rotations = 0;

    int getHeight(Node* n) {
        return n ? n->height : 0;
    }

    void updateHeight(Node* n) {
        n->height = 1 + max(getHeight(n->left), getHeight(n->right));
    }

    int getBalance(Node* n) {
        return n ? getHeight(n->left) - getHeight(n->right) : 0;
    }

    Node* rotateRight(Node* y) {
        rotations++;

        Node* x = y->left;
        Node* temp = x->right;

        x->right = y;
        y->left = temp;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        rotations++;

        Node* y = x->right;
        Node* temp = y->left;

        y->left = x;
        x->right = temp;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* search(Node* node, int id) {
        if (node == nullptr || node->rule.id == id)
            return node;

        if (id < node->rule.id)
            return search(node->left, id);

        return search(node->right, id);
    }

    Node* getMinValueNode(Node* node) {
        Node* current = node;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    Node* insert(Node* node, PacketRule rule) {
        if (!node) return new Node(rule);

        if (rule.id < node->rule.id)
            node->left = insert(node->left, rule);
        else if (rule.id > node->rule.id)
            node->right = insert(node->right, rule);
        else
            return node;

        updateHeight(node);

        int balance = getBalance(node);

        if (balance > 1 && rule.id < node->left->rule.id)
            return rotateRight(node);

        if (balance < -1 && rule.id > node->right->rule.id)
            return rotateLeft(node);

        if (balance > 1 && rule.id > node->left->rule.id) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && rule.id < node->right->rule.id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* deleteNode(Node* node, int id) {
        if (node == nullptr)
            return node;

        if (id < node->rule.id) {
            node->left = deleteNode(node->left, id);
        } 
        else if (id > node->rule.id) {
            node->right = deleteNode(node->right, id);
        } 
        else {
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;

                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }

                delete temp;
            } 
            else {
                Node* temp = getMinValueNode(node->right);
                node->rule = temp->rule;
                node->right = deleteNode(node->right, temp->rule.id);
            }
        }

        if (node == nullptr)
            return node;

        updateHeight(node);

        int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0)
            return rotateRight(node);

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0)
            return rotateLeft(node);

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }
        bool validateAVL(Node* node) {
        if (node == nullptr)
            return true;

        int balance = getBalance(node);

        if (balance < -1 || balance > 1)
            return false;

        return validateAVL(node->left) && validateAVL(node->right);
    }

public:
    void insert(PacketRule rule) {
        root = insert(root, rule);
    }

    void deleteRule(int id) {
        root = deleteNode(root, id);
    }

    PacketRule* search(int id) {
        Node* result = search(root, id);

        if (result == nullptr)
            return nullptr;

        return &result->rule;
    }

    int getRotations() {
        return rotations;
    }

    bool validateAVL() {
        return validateAVL(root);
    }

    int height() {
        return getHeight(root);
    }
};

#endif