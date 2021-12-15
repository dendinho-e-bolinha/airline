#include <iostream>

#include "BTree.h"

using namespace std;

void debugTree() {
    BTree<int> bTree(2);
    bTree.insert(4);
    bTree.insert(3);
    bTree.insert(-1);
    bTree.insert(99);
    bTree.insert(7);
    bTree.insert(12);
    bTree.insert(11);
    bTree.remove(4);
    bTree.printInorder(cout);   // -1 2 3 4 7 11 12 99

}

int main() {

    debugTree();

    return 0;
}