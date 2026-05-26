#ifndef AVLROUTERTREE_H
#define AVLROUTERTREE_H

#include "PacketRule.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <string>

class AVLRouterTree {
private:
    struct Node {
        PacketRule rule;
        Node* left;
        Node* right;
        int height;

        explicit Node(PacketRule r) : rule(r), left(NULL), right(NULL), height(1) {}
    };

    Node* root;
    int rotations;

    static std::string toString(int value) {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    static int countNodes(Node* node) {
        if (node == NULL) {
            return 0;
        }

        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    static void destroy(Node* node) {
        if (node == NULL) {
            return;
        }

        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static void updateHeight(Node* node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }

    static int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
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

    static Node* search(Node* node, int id) {
        if (node == NULL || node->rule.id == id) {
            return node;
        }

        if (id < node->rule.id) {
            return search(node->left, id);
        }

        return search(node->right, id);
    }

    static Node* getMinValueNode(Node* node) {
        Node* current = node;

        while (current->left != NULL) {
            current = current->left;
        }

        return current;
    }

    Node* insert(Node* node, PacketRule rule) {
        if (node == NULL) {
            return new Node(rule);
        }

        if (rule.id < node->rule.id) {
            node->left = insert(node->left, rule);
        } else if (rule.id > node->rule.id) {
            node->right = insert(node->right, rule);
        } else {
            return node;
        }

        updateHeight(node);

        const int balance = getBalance(node);

        if (balance > 1 && node->left != NULL && rule.id < node->left->rule.id) {
            return rotateRight(node);
        }

        if (balance < -1 && node->right != NULL && rule.id > node->right->rule.id) {
            return rotateLeft(node);
        }

        if (balance > 1 && node->left != NULL && rule.id > node->left->rule.id) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && node->right != NULL && rule.id < node->right->rule.id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* deleteNode(Node* node, int id) {
        if (node == NULL) {
            return NULL;
        }

        if (id < node->rule.id) {
            node->left = deleteNode(node->left, id);
        } else if (id > node->rule.id) {
            node->right = deleteNode(node->right, id);
        } else {
            if (node->left == NULL || node->right == NULL) {
                Node* temp = node->left ? node->left : node->right;

                if (temp == NULL) {
                    delete node;
                    return NULL;
                }

                *node = *temp;
                delete temp;
            } else {
                Node* temp = getMinValueNode(node->right);
                node->rule = temp->rule;
                node->right = deleteNode(node->right, temp->rule.id);
            }
        }

        updateHeight(node);

        const int balance = getBalance(node);

        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }

        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }

        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    static bool validateAVL(
        Node* node,
        int minId,
        int maxId,
        std::string& error,
        int& computedHeight
    ) {
        if (node == NULL) {
            computedHeight = 0;
            return true;
        }

        if (node->rule.id <= minId || node->rule.id >= maxId) {
            error = "A propriedade de BST foi violada no no " + toString(node->rule.id) + ".";
            return false;
        }

        int leftHeight = 0;
        int rightHeight = 0;

        if (!validateAVL(node->left, minId, node->rule.id, error, leftHeight)) {
            return false;
        }

        if (!validateAVL(node->right, node->rule.id, maxId, error, rightHeight)) {
            return false;
        }

        const int expectedHeight = 1 + std::max(leftHeight, rightHeight);
        const int balance = leftHeight - rightHeight;

        if (node->height != expectedHeight) {
            error =
                "A altura armazenada no no " + toString(node->rule.id) +
                " esta incorreta: esperado " + toString(expectedHeight) +
                ", obtido " + toString(node->height) + ".";
            return false;
        }

        if (balance < -1 || balance > 1) {
            error =
                "O fator de balanceamento do no " + toString(node->rule.id) +
                " saiu do intervalo [-1, 1]: " + toString(balance) + ".";
            return false;
        }

        computedHeight = expectedHeight;
        return true;
    }

public:
    AVLRouterTree() : root(NULL), rotations(0) {}

    ~AVLRouterTree() {
        destroy(root);
    }

    void insert(PacketRule rule) {
        root = insert(root, rule);
    }

    void deleteRule(int id) {
        root = deleteNode(root, id);
    }

    PacketRule* search(int id) {
        Node* result = search(root, id);
        return result == NULL ? NULL : &result->rule;
    }

    int getRotations() const {
        return rotations;
    }

    int countNodes() const {
        return countNodes(root);
    }

    int height() const {
        return getHeight(root);
    }

    bool validateAVL() const {
        std::string ignored;
        return validateAVL(ignored);
    }

    bool validateAVL(std::string& error) const {
        error.clear();

        int computedHeight = 0;
        return validateAVL(
            root,
            std::numeric_limits<int>::min(),
            std::numeric_limits<int>::max(),
            error,
            computedHeight
        );
    }
};

#endif
