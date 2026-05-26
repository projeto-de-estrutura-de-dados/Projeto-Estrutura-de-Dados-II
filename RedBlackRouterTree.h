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

    // =========================
    // ROTACAO ESQUERDA
    // =========================
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

    // =========================
    // ROTACAO DIREITA
    // =========================
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

    // =========================
    // INSERT INTERNO
    // =========================
    Node* insert(Node* root, Node* node) {

        if (root == nullptr)
            return node;

        if (node->rule.id < root->rule.id) {

            root->left = insert(root->left, node);
            root->left->parent = root;
        }

        else if (node->rule.id > root->rule.id) {

            root->right = insert(root->right, node);
            root->right->parent = root;
        }

        return root;
    }

    // =========================
    // FIX INSERT
    // =========================
    void fixInsert(Node* node) {

        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while (node != root &&
               node->color == RED &&
               node->parent->color == RED) {

            parent = node->parent;
            grandparent = parent->parent;

            // PAI NA ESQUERDA
            if (parent == grandparent->left) {

                Node* uncle = grandparent->right;

                // TIO VERMELHO
                if (uncle != nullptr &&
                    uncle->color == RED) {

                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;

                    node = grandparent;
                }

                else {

                    // ESQUERDA-DIREITA
                    if (node == parent->right) {

                        rotateLeft(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // ESQUERDA-ESQUERDA
                    rotateRight(grandparent);

                    Color temp = parent->color;
                    parent->color = grandparent->color;
                    grandparent->color = temp;

                    node = parent;
                }
            }

            // PAI NA DIREITA
            else {

                Node* uncle = grandparent->left;

                // TIO VERMELHO
                if (uncle != nullptr &&
                    uncle->color == RED) {

                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;

                    node = grandparent;
                }

                else {

                    // DIREITA-ESQUERDA
                    if (node == parent->left) {

                        rotateRight(parent);

                        node = parent;
                        parent = node->parent;
                    }

                    // DIREITA-DIREITA
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

    // =========================
    // SEARCH INTERNO
    // =========================
    Node* search(Node* node, int id) {

        if (node == nullptr ||
            node->rule.id == id)
            return node;

        if (id < node->rule.id)
            return search(node->left, id);

        return search(node->right, id);
    }

    // =========================
    // MENOR VALOR
    // =========================
    Node* minimum(Node* node) {

        while (node->left != nullptr)
            node = node->left;

        return node;
    }

    // =========================
    // ALTURA
    // =========================
    int getHeight(Node* node) {

        if (node == nullptr)
            return 0;

        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);

        return 1 + max(leftHeight, rightHeight);
    }

    // =========================
    // VALIDACAO INTERNA
    // =========================
    bool validateRedBlack(Node* node) {

        if (node == nullptr)
            return true;

        // NO VERMELHO COM FILHO VERMELHO
        if (node->color == RED) {

            if ((node->left != nullptr &&
                 node->left->color == RED) ||

                (node->right != nullptr &&
                 node->right->color == RED)) {

                return false;
            }
        }

        return validateRedBlack(node->left) &&
               validateRedBlack(node->right);
    }

    // =========================
    // FIX DELETE
    // =========================
    void fixDelete(Node* node) {

        while (node != root &&
               node != nullptr &&
               node->color == BLACK) {

            // NODE NA ESQUERDA
            if (node == node->parent->left) {

                Node* sibling = node->parent->right;

                // IRMAO VERMELHO
                if (sibling != nullptr &&
                    sibling->color == RED) {

                    sibling->color = BLACK;
                    node->parent->color = RED;

                    rotateLeft(node->parent);

                    sibling = node->parent->right;
                }

                // IRMAO PRETO
                if ((sibling->left == nullptr ||
                     sibling->left->color == BLACK) &&

                    (sibling->right == nullptr ||
                     sibling->right->color == BLACK)) {

                    sibling->color = RED;

                    node = node->parent;
                }

                else {

                    if (sibling->right == nullptr ||
                        sibling->right->color == BLACK) {

                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;

                        sibling->color = RED;

                        rotateRight(sibling);

                        sibling = node->parent->right;
                    }

                    sibling->color = node->parent->color;

                    node->parent->color = BLACK;

                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;

                    rotateLeft(node->parent);

                    node = root;
                }
            }

            // NODE NA DIREITA
            else {

                Node* sibling = node->parent->left;

                // IRMAO VERMELHO
                if (sibling != nullptr &&
                    sibling->color == RED) {

                    sibling->color = BLACK;
                    node->parent->color = RED;

                    rotateRight(node->parent);

                    sibling = node->parent->left;
                }

                // IRMAO PRETO
                if ((sibling->left == nullptr ||
                     sibling->left->color == BLACK) &&

                    (sibling->right == nullptr ||
                     sibling->right->color == BLACK)) {

                    sibling->color = RED;

                    node = node->parent;
                }

                else {

                    if (sibling->left == nullptr ||
                        sibling->left->color == BLACK) {

                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;

                        sibling->color = RED;

                        rotateLeft(sibling);

                        sibling = node->parent->left;
                    }

                    sibling->color = node->parent->color;

                    node->parent->color = BLACK;

                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;

                    rotateRight(node->parent);

                    node = root;
                }
            }
        }

        if (node != nullptr)
            node->color = BLACK;
    }

    // =========================
    // DELETE
    // =========================
    Node* deleteNode(Node* root, int id) {

        if (root == nullptr)
            return root;

        if (id < root->rule.id) {

            root->left = deleteNode(root->left, id);
        }

        else if (id > root->rule.id) {

            root->right = deleteNode(root->right, id);
        }

        else {

            // SEM FILHOS
            if (root->left == nullptr &&
                root->right == nullptr) {

                delete root;
                return nullptr;
            }

            // UM FILHO
            else if (root->left == nullptr) {

                Node* temp = root->right;

                delete root;

                return temp;
            }

            else if (root->right == nullptr) {

                Node* temp = root->left;

                delete root;

                return temp;
            }

            // DOIS FILHOS
            Node* temp = minimum(root->right);

            root->rule = temp->rule;

            root->right = deleteNode(
                root->right,
                temp->rule.id
            );
        }

        return root;
    }

public:

    // =========================
    // INSERT PUBLICO
    // =========================
    void insert(PacketRule rule) {

        Node* node = new Node(rule);

        root = insert(root, node);

        fixInsert(node);
    }

    // =========================
    // SEARCH PUBLICO
    // =========================
    PacketRule* search(int id) {

        Node* result = search(root, id);

        if (result == nullptr)
            return nullptr;

        return &result->rule;
    }

    // =========================
    // DELETE PUBLICO
    // =========================
    void deleteRule(int id) {

        Node* node = search(root, id);

        if (node == nullptr)
            return;

        root = deleteNode(root, id);

        if (root != nullptr)
            fixDelete(root);

        if (root != nullptr)
            root->color = BLACK;
    }

    // =========================
    // HEIGHT PUBLICO
    // =========================
    int height() {
        return getHeight(root);
    }

    // =========================
    // VALIDACAO PUBLICA
    // =========================
    bool validateRedBlack() {

        // RAIZ PRECISA SER PRETA
        if (root != nullptr &&
            root->color != BLACK) {

            return false;
        }

        return validateRedBlack(root);
    }

    // =========================
    // CONTADOR DE ROTACOES
    // =========================
    int getRotations() {
        return rotations;
    }
};

#endif