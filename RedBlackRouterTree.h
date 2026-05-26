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
    // ROTACAO ESQUERDA
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
    // ROTACAO DIREITA
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

            // PAI ESQUERDA
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

            // PAI DIREITA
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
    // MINIMO
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
    void fixDelete(Node* node) {

        while (node != root &&
               getColor(node) == BLACK) {

            // NODE ESQUERDA
            if (node == node->parent->left) {

                Node* sibling =
                    node->parent->right;

                // CASO 1
                if (getColor(sibling) == RED) {

                    sibling->color = BLACK;
                    node->parent->color = RED;

                    rotateLeft(node->parent);

                    sibling =
                        node->parent->right;
                }

                // CASO 2
                if (getColor(sibling->left) == BLACK &&
                    getColor(sibling->right) == BLACK) {

                    sibling->color = RED;

                    node = node->parent;
                }

                else {

                    // CASO 3
                    if (getColor(sibling->right)
                        == BLACK) {

                        if (sibling->left != nullptr)
                            sibling->left->color =
                                BLACK;

                        sibling->color = RED;

                        rotateRight(sibling);

                        sibling =
                            node->parent->right;
                    }

                    // CASO 4
                    sibling->color =
                        node->parent->color;

                    node->parent->color =
                        BLACK;

                    if (sibling->right != nullptr)
                        sibling->right->color =
                            BLACK;

                    rotateLeft(node->parent);

                    node = root;
                }
            }

            // NODE DIREITA
            else {

                Node* sibling =
                    node->parent->left;

                // CASO 1
                if (getColor(sibling) == RED) {

                    sibling->color = BLACK;
                    node->parent->color = RED;

                    rotateRight(node->parent);

                    sibling =
                        node->parent->left;
                }

                // CASO 2
                if (getColor(sibling->left) == BLACK &&
                    getColor(sibling->right) == BLACK) {

                    sibling->color = RED;

                    node = node->parent;
                }

                else {

                    // CASO 3
                    if (getColor(sibling->left)
                        == BLACK) {

                        if (sibling->right != nullptr)
                            sibling->right->color =
                                BLACK;

                        sibling->color = RED;

                        rotateLeft(sibling);

                        sibling =
                            node->parent->left;
                    }

                    // CASO 4
                    sibling->color =
                        node->parent->color;

                    node->parent->color =
                        BLACK;

                    if (sibling->left != nullptr)
                        sibling->left->color =
                            BLACK;

                    rotateRight(node->parent);

                    node = root;
                }
            }
        }

        if (node != nullptr)
            node->color = BLACK;
    }

    // ====================================
    // DELETE
    // ====================================
    void deleteNode(Node* z) {

        Node* y = z;
        Node* x = nullptr;

        Color originalColor = y->color;

        // SEM FILHO ESQUERDO
        if (z->left == nullptr) {

            x = z->right;

            transplant(z, z->right);
        }

        // SEM FILHO DIREITO
        else if (z->right == nullptr) {

            x = z->left;

            transplant(z, z->left);
        }

        // DOIS FILHOS
        else {

            y = minimum(z->right);

            originalColor = y->color;

            x = y->right;

            if (y->parent == z) {

                if (x != nullptr)
                    x->parent = y;
            }

            else {

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

        if (originalColor == BLACK &&
            x != nullptr) {

            fixDelete(x);
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

        return 1 +
               max(
                   getHeight(node->left),
                   getHeight(node->right)
               );
    }

    // ====================================
    // VALIDACAO VERMELHO
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
    // HEIGHT PUBLICO
    // ====================================
    int height() {
        return getHeight(root);
    }

    // ====================================
    // VALIDACAO PUBLICA
    // ====================================
    bool validateRedBlack() {

        if (root == nullptr)
            return true;

        // RAIZ PRECISA SER PRETA
        if (root->color != BLACK)
            return false;

        // VALIDAR FILHOS VERMELHOS
        if (!validateRed(root))
            return false;

        // VALIDAR BLACK HEIGHT
        if (blackHeight(root) == 0)
            return false;

        return true;
    }

    // ====================================
    // ROTACOES
    // ====================================
    int getRotations() {
        return rotations;
    }
};

#endif