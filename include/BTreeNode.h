#ifndef PROJ_1_BTREENODE_H
#define PROJ_1_BTREENODE_H

template <class T>
struct BTNode {
    T data;
    BTNode *left, *right;

    BTNode(const T &data, BTNode *left = nullptr, BTNode *right = nullptr) {
        this->data = data;
        this->left = left;
        this->right = right;
    }
};

#endif //PROJ_1_BTREENODE_H
