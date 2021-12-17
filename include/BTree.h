#ifndef AIRLINE_BTREE_H
#define AIRLINE_BTREE_H

// STL imports
#include <ostream>
#include <stdexcept>


/**
 * @Brief Represents a node of the tree
 *
 * @tparam T type of the data stored in each node
 * @param *left Pointer to the left child node
 * @param *right Pointer to the right child node
 */
template <class T>
struct BTNode {
    T data;
    BTNode *left, *right;


    /**
     * @brief Constructor for a `BTNode`
     * @details if `left` or `right` are not passed, it is assumed that the node is a leaf
     */
    BTNode(const T &data, BTNode *left = nullptr, BTNode *right = nullptr) {
        this->data = data;
        this->left = left;
        this->right = right;
    }
};

/**
 * @brief Binary Search tree
 * @tparam T type of the data stored in each node
 *
 * @details For each node n, n->left->data < n->data && n->right->data > n->data
 */
template <class T>
class BTree {
private:
    BTNode<T> *root;

    /**
     * @brief Destroys a tree
     * @param node starting node
     */
    void clear(BTNode<T> *node);

    /**
     * @brief Copies a tree
     *
     * @param node starting node
     * @return Returns the root node of the copy tree
     */
    BTNode<T> *copyTree(const BTNode<T> *node) const;

    /**
     * @brief Inserts a new node with the given value on a tree
     *
     * @param key value to be inserted
     * @param node starting node
     */
    bool insert(T key, BTNode<T> *node);


    /**
     * @brief Finds the first node with a given value on a tree
     *
     * @param key value to be searched
     * @param node starting node
     *
     * @return Returns the node that contains the value `key`
     */
    BTNode<T> *search(T key, BTNode<T> *node);

    /**
     * @brief Finds the node with the highest value on the tree
     *
     * @param node starting node
     *
     * @return Returns the node that contains the highest value
     */
    BTNode<T> *max(BTNode<T> *node);


    /**
     * @brief Finds the node with the lower value on the tree
     *
     * @param node starting node
     *
     * @return Returns the node that contains the lower value
     */
    BTNode<T> *min(BTNode<T> *node);

    /**
     * @brief Removes the first node of the tree with the value `k`
     * @param node starting node
     * @param key value to be removed
     * @return Returns the new root node
     */
    BTNode<T> *remove(BTNode<T> *node, T key);

    /**
     * @brief Displays the tree data in a Preorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printPreorder(std::ostream &out, const BTNode<T> *node) const;


    /**
     * @brief Displays the tree data in a Postorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printPostorder(std::ostream &out, const BTNode<T> *node) const;


    /**
     * @brief Displays the tree data in a Inorder format
     *
     * @param out ostream
     * @param node starting node
     */
    void printInorder(std::ostream &out, const BTNode<T> *node) const;

public:

    /**
     * @brief default constructor
     */
    BTree();

    /**
     * @brief Constructs a tree with a single root node that has the given value
     * @param el value for the root data
     */
    BTree(const T& el);

    /**
     * @brief Copy constructor
     * @param bTree Tree to be copied
     */
    BTree(const BTree<T>& bTree);

    /**
     * @brief Destructor
     * @details Deallocates all the memory allocated to the BTree object
     */
    ~BTree();

    /**
     * @brief Destroys the tree and deallocates all the memory associated to it
     */
    void clear();

    /**
     * @brief Retrieves the root BTNode
     * @return Returns a pointer to the tree's root
     */
    BTNode<T> *getRoot() const;

    /**
     * @brief Checks if the tree is empty
     *
     * @return Returns a boolean that represents the tree being empty (true) or not (false)
     */
    bool isEmpty() const;

     /**
      * @brief Inserts a value on the tree
      * @param key value to be inserted
      *
      * @param key
      * @return Returns `true` if `key` was inserted and false if it already existed on the tree
      */
    bool insert(T key);

    /**
     * @brief Removes the first node of the tree with the value `k`
     * @param key value to be removed
     * @return Returns true if the node was found and false if it does not exist on the tree
     *
     * @note the children nodes will also be removed
     */
    void remove(T val);

    /**
     * @brief Searches a value on the key
     * @param key value to be searched
     *
     * @return Returns the first node that has the value `key`
     */
    BTNode<T> *search(T key);

    /**
     * @brief Displays the tree data in a Preorder format
     *
     * @param out ostream
     */
    void printPreorder(std::ostream &out) const;

    /**
     * @brief Displays the tree data in a Postorder format
     *
     * @param out ostream
     */
    void printPostorder(std::ostream &out) const;

    /**
     * @brief Displays the tree data in an Inorder format
     *
     * @param out ostream
     */
    void printInorder(std::ostream &out) const;
};


template<class T>
void BTree<T>::clear(BTNode<T> *node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template<class T>
BTNode<T> *BTree<T>::copyTree(const BTNode<T> *node) const {
    if (node)
        return new BTNode<T>(node->data, copyTree(node->left), copyTree(node->right));
    else
        return NULL;

}

template<class T>
bool BTree<T>::insert(T key, BTNode<T> *node) {
    bool success = false;
    if (key < node->data) {
        if (node->left) {
            insert(key, node->left);
        } else {
            node->left = new BTNode<T>(key);
            success = true;
        }
    } else if (node->data < key) {
        if (node->right) {
            insert(key, node->right);
        } else {
            node->right = new BTNode<T>(key);
            success = true;
        }
    }
    return success;
}

template<class T>
BTNode<T> *BTree<T>::search(T key, BTNode<T> *node) {
    if (node) {
        if (key < node->data)
            return search(key, node->left);
        else if (node->data < key)
            return search(key, node->right);
        else
            return node;
    } else {
        return nullptr;
    }
}

template<class T>
void BTree<T>::printPreorder(std::ostream &out, const BTNode<T> *node) const {
    if (node) {
        out << node->data << ' ';
        printPreorder(out, node->left);
        printPreorder(out, node->right);
    }
}

template<class T>
void BTree<T>::printPostorder(std::ostream &out, const BTNode<T> *node) const {
    if (node) {
        printPostorder(out, node->left);
        printPostorder(out, node->right);
        out << node->data << ' ';
    }
}

template<class T>
void BTree<T>::printInorder(std::ostream &out, const BTNode<T> *node) const {
    if (node) {
        printInorder(out, node->left);
        out << node->data << ' ';
        printInorder(out, node->right);
    }
}

template<class T>
BTree<T>::BTree() { root = nullptr; }

template<class T>
BTree<T>::BTree(const T &el) { root = new BTNode<T>(el); }

template<class T>
BTree<T>::BTree(const BTree<T> &bTree) { root = copyTree(bTree.root); }

template<class T>
void BTree<T>::clear() {
    clear(root);
    root = nullptr;
}

template<class T>
BTNode<T> *BTree<T>::getRoot() const {
    return root;
}

template<class T>
bool BTree<T>::isEmpty() const { return root == NULL; }

template<class T>
BTree<T>::~BTree() { clear(); }

template<class T>
bool BTree<T>::insert(T key) {
    bool success = false;
    if (root)
        success = insert(key, root);
    else
        root = new BTNode<T>(key);
    return success;
}

template<class T>
BTNode<T> *BTree<T>::search(T key) {
    return search(key, root);
}

template<class T>
void BTree<T>::printPreorder(std::ostream &out) const {
    printPreorder(out, root);
    out << "\n";
}

template<class T>
void BTree<T>::printPostorder(std::ostream &out) const {
    printPostorder(out, root);
    out << "\n";
}

template<class T>
void BTree<T>::printInorder(std::ostream &out) const {
    printInorder(out, root);
    out << "\n";
}

template<class T>
BTNode<T> *BTree<T>::max(BTNode<T> *node) {
    if (node == NULL)
        return NULL;
    else if (node->right == NULL)
        return node;
    else
        return max(node->right);
}

template<class T>
BTNode<T> *BTree<T>::min(BTNode<T> *node) {
    if (node == NULL)
        return NULL;
    else if (node->left == NULL)
        return node;
    else
        return min(node->left);
}

template<class T>
BTNode<T> *BTree<T>::remove(BTNode<T> *node, T key) {
    BTNode<T> *tmp;
    if (node == NULL) {
        return NULL;
    } else if (key < node->data) {
        node->left = remove(node->left, key);
    } else if (node->data < key) {
        node->right = remove(node->right, key);
    } else if (node->left && node->right) {
        tmp = min(node->right);
        node->data = tmp->data;
        node->right = remove(node->right, node->data);
    } else {
        tmp = node;
        if (node->left == NULL) {
            node = node->left;
        } else if (node->right == NULL) {
            node = node->right;
        }
        delete tmp;
    }
    return node;
}

template<class T>
void BTree<T>::remove(T val) {
    root = remove(root, val);
}

#endif //AIRLINE_BTREE_H