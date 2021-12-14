#include <iostream>

#include "BTree.h"

using namespace std;

int main() {
    BTree<int> bTree(2);
    bTree.insert(4);
    bTree.insert(3);
    bTree.insert(-1);
    bTree.insert(99);
    bTree.insert(7);
    bTree.insert(12);
    bTree.insert(11);
    cout << bTree.min(bTree.getRoot())->data << endl;
    cout << bTree.max(bTree.getRoot())->data << endl;
    BTNode<int> *node = bTree.search(99);
    bTree.remover(99);

    //bTree.remover(99);


    bTree.printInorder(cout);   // -1 2 3 4 4 7 11 12 99

    return 0;
}