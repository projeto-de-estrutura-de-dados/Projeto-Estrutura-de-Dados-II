#ifndef REDBLACKROUTERTREE_H
#define REDBLACKROUTERTREE_H

#include "PacketRule.h"
#include <algorithm>

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

        Node(PacketRule r)
            : rule(r),
              color(RED),
              left(nullptr),
              right(nullptr),
              parent(nullptr) {}
    };

    Node* root = nullptr;
    int rotations = 0;

    // ====================================
    // COR SEGURA
    // ====================================
    Color getColor(Node* node) {

        if (node == nullptr)
            return BLACK;

        return node->color;
    }

    // ====================================
    // ROTAÇÃO ESQUERDA
    // ====================================
    void rotateLeft(Node* x) {

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
    }

    // ====================================
    // ROTAÇÃO DIREITA
    // ====================================
    void rotateRight(Node* y) {

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
    }

    // ====================================
    // INSERT BST
    // ====================================
    Node* insert(Node* current, Node* node) {

        if (current == nullptr)
            return node;

        if (node->rule.id < current->rule.id) {

            current->left = insert(current->left, node);
            current->left->parent = current;
        }

        else if (node->rule.id > current->rule.id) {

            current->right = insert(current->right, node);
            current->right->parent = current;
        }

        return current;
    }

    // ====================================
    // FIX INSERT
    // ====================================
    void fixInsert(Node* node) {

        while (node != root &&
               getColor(node) == RED &&
               getColor(node->parent) == RED) {

            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            // PAI À ESQUERDA
            if (parent == grandparent->left) {

                Node* uncle = grandparent->right;

                // CASO 1
                if (getColor(uncle) == RED) {

                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;

                    node = grandparent;
                }

                else {

                    // CASO 2
                    if (node == parent->right) {

                        rotateLeft(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // CASO 3
                    rotateRight(grandparent);

                    swap(parent->color,
                         grandparent->color);

                    node = parent;
                }
            }

            // PAI À DIREITA
            else {

                Node* uncle = grandparent->left;

                // CASO 1
                if (getColor(uncle) == RED) {

                    parent->color = BLACK;
                    uncle->color = BLACK;
                    grandparent->color = RED;

                    node = grandparent;
                }

                else {

                    // CASO 2
                    if (node == parent->left) {

                        rotateRight(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // CASO 3
                    rotateLeft(grandparent);

                    swap(parent->color,
                         grandparent->color);

                    node = parent;
                }
            }
        }

        if (root != nullptr)
            root->color = BLACK;
    }

    // ====================================
    // SEARCH
    // ====================================
    Node* search(Node* node, int id) {

        if (node == nullptr ||
            node->rule.id == id)
            return node;

        if (id < node->rule.id)
            return search(node->left, id);

        return search(node->right, id);
    }

    // ====================================
    // MENOR NÓ
    // ====================================
    Node* minimum(Node* node) {

        while (node->left != nullptr)
            node = node->left;

        return node;
    }

    // ====================================
    // TRANSPLANT
    // ====================================
    void transplant(Node* u, Node* v) {

        if (u->parent == nullptr)
            root = v;

        else if (u == u->parent->left)
            u->parent->left = v;

        else
            u->parent->right = v;

        if (v != nullptr)
            v->parent = u->parent;
    }

    // ====================================
    // FIX DELETE
    // ====================================
    void fixDelete(Node* node, Node* parent) {

        while (node != root &&
               getColor(node) == BLACK) {

            if (parent == nullptr)
                break;

            // NODE À ESQUERDA
            if (node == parent->left) {

                Node* sibling = parent->right;

                // CASO 1
                if (getColor(sibling) == RED) {

                    sibling->color = BLACK;
                    parent->color = RED;

                    rotateLeft(parent);

                    sibling = parent->right;
                }

                // SIBLING NULO
                if (sibling == nullptr) {

                    node = parent;
                    parent = node->parent;

                    continue;
                }

                // CASO 2
                if (getColor(sibling->left) == BLACK &&
                    getColor(sibling->right) == BLACK) {

                    sibling->color = RED;

                    node = parent;
                    parent = node->parent;
                }

                else {

                    // CASO 3
                    if (getColor(sibling->right) == BLACK) {

                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;

                        sibling->color = RED;

                        rotateRight(sibling);

                        sibling = parent->right;
                    }

                    // CASO 4
                    sibling->color = parent->color;

                    parent->color = BLACK;

                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;

                    rotateLeft(parent);

                    node = root;
                }
            }

            // NODE À DIREITA
            else {

                Node* sibling = parent->left;

                // CASO 1
                if (getColor(sibling) == RED) {

                    sibling->color = BLACK;
                    parent->color = RED;

                    rotateRight(parent);

                    sibling = parent->left;
                }

                // SIBLING NULO
                if (sibling == nullptr) {

                    node = parent;
                    parent = node->parent;

                    continue;
                }

                // CASO 2
                if (getColor(sibling->left) == BLACK &&
                    getColor(sibling->right) == BLACK) {

                    sibling->color = RED;

                    node = parent;
                    parent = node->parent;
                }

                else {

                    // CASO 3
                    if (getColor(sibling->left) == BLACK) {

                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;

                        sibling->color = RED;

                        rotateLeft(sibling);

                        sibling = parent->left;
                    }

                    // CASO 4
                    sibling->color = parent->color;

                    parent->color = BLACK;

                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;

                    rotateRight(parent);

                    node = root;
                }
            }

            if (node != nullptr)
                parent = node->parent;
        }

        if (node != nullptr)
            node->color = BLACK;
    }

    // ====================================
    // DELETE NODE
    // ====================================
    void deleteNode(Node* z) {

        Node* y = z;
        Node* x = nullptr;
        Node* xParent = nullptr;

        Color originalColor = y->color;

        // SEM FILHO ESQUERDO
        if (z->left == nullptr) {

            x = z->right;
            xParent = z->parent;

            transplant(z, z->right);
        }

        // SEM FILHO DIREITO
        else if (z->right == nullptr) {

            x = z->left;
            xParent = z->parent;

            transplant(z, z->left);
        }

        // DOIS FILHOS
        else {

            y = minimum(z->right);

            originalColor = y->color;

            x = y->right;

            if (y->parent == z) {

                xParent = y;

                if (x != nullptr)
                    x->parent = y;
            }

            else {

                xParent = y->parent;

                transplant(y, y->right);

                y->right = z->right;

                if (y->right != nullptr)
                    y->right->parent = y;
            }

            transplant(z, y);

            y->left = z->left;

            if (y->left != nullptr)
                y->left->parent = y;

            y->color = z->color;
        }

        delete z;

        // CORREÇÃO DO BLACK HEIGHT
        if (originalColor == BLACK) {

            fixDelete(x, xParent);
        }

        if (root != nullptr)
            root->color = BLACK;
    }

    // ====================================
    // ALTURA
    // ====================================
    int getHeight(Node* node) {

        if (node == nullptr)
            return 0;

        return 1 + max(
            getHeight(node->left),
            getHeight(node->right)
        );
    }

    // ====================================
    // VALIDAÇÃO RED
    // ====================================
    bool validateRed(Node* node) {

        if (node == nullptr)
            return true;

        if (node->color == RED) {

            if (getColor(node->left) == RED ||
                getColor(node->right) == RED)
                return false;
        }

        return validateRed(node->left) &&
               validateRed(node->right);
    }

    // ====================================
    // BLACK HEIGHT
    // ====================================
    int blackHeight(Node* node) {

        if (node == nullptr)
            return 1;

        int left =
            blackHeight(node->left);

        int right =
            blackHeight(node->right);

        if (left == 0 ||
            right == 0 ||
            left != right)
            return 0;

        if (node->color == BLACK)
            return left + 1;

        return left;
    }

public:

    // ====================================
    // INSERT PUBLICO
    // ====================================
    void insert(PacketRule rule) {

        Node* node = new Node(rule);

        root = insert(root, node);

        fixInsert(node);
    }

    // ====================================
    // SEARCH PUBLICO
    // ====================================
    PacketRule* search(int id) {

        Node* result =
            search(root, id);

        if (result == nullptr)
            return nullptr;

        return &result->rule;
    }

    // ====================================
    // DELETE PUBLICO
    // ====================================
    void deleteRule(int id) {

        Node* node =
            search(root, id);

        if (node == nullptr)
            return;

        deleteNode(node);
    }

    // ====================================
    // HEIGHT
    // ====================================
    int height() {
        return getHeight(root);
    }

    // ====================================
    // VALIDATE
    // ====================================
    bool validateRedBlack() {

        if (root == nullptr)
            return true;

        // RAIZ PRETA
        if (root->color != BLACK)
            return false;

        // SEM VERMELHO DUPLO
        if (!validateRed(root))
            return false;

        // BLACK HEIGHT
        if (blackHeight(root) == 0)
            return false;

        return true;
    }

    // ====================================
    // ROTAÇÕES
    // ====================================
    int getRotations() {
        return rotations;
    }
};

#endif