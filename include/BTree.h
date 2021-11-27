#ifndef PROJ_1_BTREE_H
#define PROJ_1_BTREE_H

#include <vector>
#include <ostream>
#include <stack>
#include <stdexcept>

#include "BTreeNode.h"

using namespace std;


/**
 * @brief Binary Search tree
 * @tparam T - type of the data stored in each node
 *
 * @details For each node n, n->left->data < n->data && n->right->data >= n->data
 */
template <class T>
class BTree {

private:
    BTNode<T> *root;


    void clear(BTNode<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }


    BTNode<T> *copyTree(const BTNode<T> *node) const {
        if (node) {
            BTNode<T> *newNode = new BTNode<T>(node->data, node->left, node->right);
            return node;
        }
        return nullptr;
    }


    void insert(T key, BTNode<T> *node) {
        if (key < node->data) {
            if (node->left) {
                insert(key, node->left);
            } else {
                node->left = new BTNode<T>(key);
            }
        } else if (key >= node->data) {
            if (node->right) {
                insert(key, node->right);
            } else {
                node->right = new BTNode<T>(key);
            }
        }
    }


    BTNode<T> *search(T key, BTNode<T> *node) {
        if (node) {
            if (key == node->data)
                return node;
            else if (key < node->data)
                return search(key, node->left);
            else
                return search(key, node->right);
        } else {
            return nullptr;
        }
    }


    void printPreorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            out << node->data << ' ';
            printPreorder(out, node->left);
            printPreorder(out, node->right);
        }
    }


    void printPostorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            printPostorder(out, node->left);
            printPostorder(out, node->right);
            out << node->data << ' ';
        }
    }


    void printInorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            printInorder(out, node->left);
            out << node->data << ' ';
            printInorder(out, node->right);
        }
    }

public:
    BTree() { root = nullptr; }
    BTree(const T& el) { root = new BTNode<T>(el); }
    BTree(const BTree<T>& bTree) { root = copyTree(bTree.root); }


    ~BTree() { clear(); }


    void clear() {
        clear(root);
        root = nullptr;
    }


    BTNode<T> *getRoot() const {
        return root;
        if(root) return root;
        throw std::underflow_error("BTree instance must have a not NULL value");
    }


    bool isEmpty() const { return root == nullptr; }


    void insert(T key) {
        if (root)
            insert(key, root);
        else
            root = new BTNode<T>(key);
    }


    BTNode<T> *search(T key) {
        return search(key, root);
    }


    void printPreorder(ostream &out) const {
        printPreorder(out, root);
        out << "\n";
    }


    void printPostorder(ostream &out) const {
        printPostorder(out, root);
        out << "\n";
    }


    void printInorder(ostream &out) const {
        printInorder(out, root);
        out << "\n";
    }
};

#endif //PROJ_1_BTREE_H
