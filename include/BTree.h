#ifndef PROJ_1_BTREE_H
#define PROJ_1_BTREE_H

// STL imports
#include <ostream>
#include <stdexcept>

// Files
#include "BTreeNode.h"

using namespace std;

/**
 * @brief Binary Search tree
 * @tparam T type of the data stored in each node
 *
 * @details For each node n, n->left->data < n->data && n->right->data >= n->data
 */
template <class T>
class BTree {
private:
    BTNode<T> *root;


    /**
     * @brief Destroys a tree
     * @param node starting node
     */
    void clear(BTNode<T> *node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }


    /**
     * @brief Copies a tree
     *
     * @param node starting node
     * @return Returns the root node of the copy tree
     */
    BTNode<T> *copyTree(const BTNode<T> *node) const {
        if (node) {
            BTNode<T> *newNode = new BTNode<T>(node->data, node->left, node->right);
            return node;
        }
        return nullptr;
    }


    /**
     * @brief Inserts a new node with the given value on a tree
     *
     * @param key value to be inserted
     * @param node starting node
     */
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


    /**
     * @brief Finds the first node with a given value on a tree
     *
     * @param key value to be searched
     * @param node starting node
     *
     * @return Returns the node that contains the value `key`
     */
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


    /**
     * @brief Displays the tree data in a Preorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printPreorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            out << node->data << ' ';
            printPreorder(out, node->left);
            printPreorder(out, node->right);
        }
    }


    /**
     * @brief Displays the tree data in a Postorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printPostorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            printPostorder(out, node->left);
            printPostorder(out, node->right);
            out << node->data << ' ';
        }
    }


    /**
     * @brief Displays the tree data in a Inorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printInorder(ostream &out, const BTNode<T> *node) const {
        if (node) {
            printInorder(out, node->left);
            out << node->data << ' ';
            printInorder(out, node->right);
        }
    }

public:

    /**
     * @brief default constructor
     */
    BTree() { root = nullptr; }


    /**
     * @brief Constructs a tree with a single root node that has the given value
     * @param el value for the root data
     */
    BTree(const T& el) { root = new BTNode<T>(el); }

    /**
     * @brief Copy constructor
     * @param bTree Tree to be copied
     */
    BTree(const BTree<T>& bTree) { root = copyTree(bTree.root); }


    /**
     * @brief Destructor
     * @details Deallocates all the memory allocated to the BTree object
     */
    ~BTree() { clear(); }


    /**
     * @brief Destroys the tree and deallocates all the memory associated to it
     */
    void clear() {
        clear(root);
        root = nullptr;
    }


    /**
     * @brief Retrieves the root BTNode
     * @return Returns a pointer to the tree's root
     */
    BTNode<T> *getRoot() const {
        return root;
    }


    /**
     * @brief Checks if the tree is empty
     *
     * @return Returns a boolean that represents the tree being empty (true) or not (false)
     */
    bool isEmpty() const { return root == nullptr; }


    /**
     * @brief Inserts a value on the tree
     * @param key value to be inserted
     */
    void insert(T key) {
        if (root)
            insert(key, root);
        else
            root = new BTNode<T>(key);
    }


    /**
     * @brief Searches a value on the key
     * @param key value to be searched
     *
     * @return Returns the first node that has the value `key`
     */
    BTNode<T> *search(T key) {
        return search(key, root);
    }


    /**
     * @brief Displays the tree data in a Preorder format
     *
     * @param out ostream
     */
    void printPreorder(ostream &out) const {
        printPreorder(out, root);
        out << "\n";
    }


    /**
     *
     * @param out
     */
    /**
     * @brief Displays the tree data in a Postorder format
     *
     * @param out ostream
     */
    void printPostorder(ostream &out) const {
        printPostorder(out, root);
        out << "\n";
    }


    /**
     * @brief Displays the tree data in an Inorder format
     *
     * @param out ostream
     */
    void printInorder(ostream &out) const {
        printInorder(out, root);
        out << "\n";
    }
};

#endif //PROJ_1_BTREE_H
