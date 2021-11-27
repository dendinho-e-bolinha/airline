#ifndef PROJ_1_BTREENODE_H
#define PROJ_1_BTREENODE_H

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

#endif //PROJ_1_BTREENODE_H
