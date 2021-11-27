#include <iostream>

#include "BTree.h"

using namespace std;

int main() {
    BTree<int> bTree{};
    bTree.insert(4);
    bTree.insert(5);
    bTree.insert(6);
    bTree.insert(7);
    bTree.insert(12);
    bTree.insert(9);
    bTree.insert(10);
    bTree.insert(20);
    bTree.insert(31);

    bTree.printInorder(cout);
    bTree.printPreorder(cout);
    bTree.printPostorder(cout);

    cout << bTree.isEmpty() << endl;

    BTNode<int> *node = bTree.search(12);

    cout << node->data << " " << node->left->data << " " << node->right->data << endl;

    bTree.clear();

    cout << bTree.isEmpty() << endl;

    return 0;
}