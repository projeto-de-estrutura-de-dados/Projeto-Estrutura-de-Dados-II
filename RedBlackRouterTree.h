#ifndef REDBLACKROUTERTREE_H
#define REDBLACKROUTERTREE_H

#include "PacketRule.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <string>

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

        explicit Node(PacketRule r) : rule(r), color(RED), left(NULL), right(NULL), parent(NULL) {}
    };

    Node* root;
    int rotations;

    static std::string toString(int value) {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static void destroy(Node* node) {
        if (node == NULL) {
            return;
        }

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static int countNodes(Node* node) {
        if (node == NULL) {
            return 0;
        }

        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    Node* rotateLeft(Node* x) {
        rotations++;

        Node* y = x->right;
        x->right = y->left;

        if (y->left != NULL) {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == NULL) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;

        return y;
    }

    Node* rotateRight(Node* y) {
        rotations++;

        Node* x = y->left;
        y->left = x->right;

        if (x->right != NULL) {
            x->right->parent = y;
        }

        x->parent = y->parent;

        if (y->parent == NULL) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }

        x->right = y;
        y->parent = x;

        return x;
    }

    static Node* search(Node* node, int id) {
        if (node == NULL || node->rule.id == id) {
            return node;
        }

        if (id < node->rule.id) {
            return search(node->left, id);
        }

        return search(node->right, id);
    }

    static Node* minimum(Node* node) {
        while (node->left != NULL) {
            node = node->left;
        }

        return node;
    }

    Node* insert(Node* currentRoot, Node* node) {
        if (currentRoot == NULL) {
            return node;
        }

        if (node->rule.id < currentRoot->rule.id) {
            currentRoot->left = insert(currentRoot->left, node);
            currentRoot->left->parent = currentRoot;
        } else if (node->rule.id > currentRoot->rule.id) {
            currentRoot->right = insert(currentRoot->right, node);
            currentRoot->right->parent = currentRoot;
        }

        return currentRoot;
    }

    void fixInsert(Node* node) {
        while (node != root && node->color == RED && node->parent->color == RED) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle != NULL && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateRight(grandparent);

                    const Color temp = parent->color;
                    parent->color = grandparent->color;
                    grandparent->color = temp;
                    node = parent;
                }
            } else {
                Node* uncle = grandparent->left;

                if (uncle != NULL && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateLeft(grandparent);

                    const Color temp = parent->color;
                    parent->color = grandparent->color;
                    grandparent->color = temp;
                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

    void fixDelete(Node* node) {
        while (node != root && node != NULL && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;

                if (sibling != NULL && sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }

                if (sibling == NULL) {
                    node = node->parent;
                    continue;
                }

                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->right == NULL || sibling->right->color == BLACK) {
                        if (sibling->left != NULL) {
                            sibling->left->color = BLACK;
                        }

                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;

                    if (sibling->right != NULL) {
                        sibling->right->color = BLACK;
                    }

                    rotateLeft(node->parent);
                    node = root;
                }
            } else {
                Node* sibling = node->parent->left;

                if (sibling != NULL && sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }

                if (sibling == NULL) {
                    node = node->parent;
                    continue;
                }

                if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                    (sibling->right == NULL || sibling->right->color == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                } else {
                    if (sibling->left == NULL || sibling->left->color == BLACK) {
                        if (sibling->right != NULL) {
                            sibling->right->color = BLACK;
                        }

                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }

                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;

                    if (sibling->left != NULL) {
                        sibling->left->color = BLACK;
                    }

                    rotateRight(node->parent);
                    node = root;
                }
            }
        }

        if (node != NULL) {
            node->color = BLACK;
        }
    }

    Node* deleteNode(Node* currentRoot, int id) {
        if (currentRoot == NULL) {
            return NULL;
        }

        if (id < currentRoot->rule.id) {
            currentRoot->left = deleteNode(currentRoot->left, id);
        } else if (id > currentRoot->rule.id) {
            currentRoot->right = deleteNode(currentRoot->right, id);
        } else {
            if (currentRoot->left == NULL && currentRoot->right == NULL) {
                delete currentRoot;
                return NULL;
            }

            if (currentRoot->left == NULL) {
                Node* temp = currentRoot->right;
                delete currentRoot;
                return temp;
            }

            if (currentRoot->right == NULL) {
                Node* temp = currentRoot->left;
                delete currentRoot;
                return temp;
            }

            Node* temp = minimum(currentRoot->right);
            currentRoot->rule = temp->rule;
            currentRoot->right = deleteNode(currentRoot->right, temp->rule.id);
        }

        return currentRoot;
    }

    static int getHeight(Node* node) {
        if (node == NULL) {
            return 0;
        }

        return 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    static bool validateRedBlack(
        Node* node,
        Node* expectedParent,
        int minId,
        int maxId,
        int blackCount,
        int& expectedBlackHeight,
        std::string& error
    ) {
        if (node == NULL) {
            const int finalBlackHeight = blackCount + 1;

            if (expectedBlackHeight == -1) {
                expectedBlackHeight = finalBlackHeight;
                return true;
            }

            if (expectedBlackHeight != finalBlackHeight) {
                error =
                    "Black-height inconsistente: esperado " +
                    toString(expectedBlackHeight) + ", encontrado " +
                    toString(finalBlackHeight) + ".";
                return false;
            }

            return true;
        }

        if (node->parent != expectedParent) {
            error = "O ponteiro parent do no " + toString(node->rule.id) + " esta incorreto.";
            return false;
        }

        if (node->rule.id <= minId || node->rule.id >= maxId) {
            error = "A propriedade de BST foi violada no no " + toString(node->rule.id) + ".";
            return false;
        }

        if (node->color == RED) {
            if ((node->left != NULL && node->left->color == RED) ||
                (node->right != NULL && node->right->color == RED)) {
                error =
                    "Existe um no vermelho com filho vermelho na chave " +
                    toString(node->rule.id) + ".";
                return false;
            }
        } else {
            blackCount++;
        }

        if (!validateRedBlack(
                node->left,
                node,
                minId,
                node->rule.id,
                blackCount,
                expectedBlackHeight,
                error
            )) {
            return false;
        }

        return validateRedBlack(
            node->right,
            node,
            node->rule.id,
            maxId,
            blackCount,
            expectedBlackHeight,
            error
        );
    }

public:
    RedBlackRouterTree() : root(NULL), rotations(0) {}

    ~RedBlackRouterTree() {
        destroy(root);
    }

    void insert(PacketRule rule) {
        Node* node = new Node(rule);
        root = insert(root, node);
        fixInsert(node);
    }

    PacketRule* search(int id) {
        Node* result = search(root, id);
        return result == NULL ? NULL : &result->rule;
    }

    void deleteRule(int id) {
        Node* node = search(root, id);

        if (node == NULL) {
            return;
        }

        root = deleteNode(root, id);

        if (root != NULL) {
            fixDelete(root);
            root->color = BLACK;
            root->parent = NULL;
        }
    }

    int height() const {
        return getHeight(root);
    }

    int countNodes() const {
        return countNodes(root);
    }

    int getRotations() const {
        return rotations;
    }

    int blackHeight() const {
        if (root == NULL) {
            return 1;
        }

        int expectedBlackHeight = -1;
        std::string ignored;
        validateRedBlack(
            root,
            NULL,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            0,
            expectedBlackHeight,
            ignored
        );
        return expectedBlackHeight;
    }

    bool validateRedBlack() const {
        std::string ignored;
        return validateRedBlack(ignored);
    }

    bool validateRedBlack(std::string& error) const {
        error.clear();

        if (root == NULL) {
            return true;
        }

        if (root->color != BLACK) {
            error = "A raiz da Red-Black precisa ser preta.";
            return false;
        }

        if (root->parent != NULL) {
            error = "A raiz precisa ter parent nulo.";
            return false;
        }

        int expectedBlackHeight = -1;
        return validateRedBlack(
            root,
            NULL,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            0,
            expectedBlackHeight,
            error
        );
    }
};

#endif
