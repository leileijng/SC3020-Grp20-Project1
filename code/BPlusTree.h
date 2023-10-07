#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include "Address.h"
using namespace std;

// BP node
class Node {
  private:
    bool IS_LEAF;
    long long *key;
    int size;
    Node **ptr;
    Address **bptr;

  public:
    Node();

  friend class BPTree;
};

// BP tree
class BPTree {
  private:
    Node *root;
    int MAX; // Parameter n of the B+ tree
    int numNodes; // Number of nodes in the B+ tree
    void insertInternal(long long, Node *, Node *);
    void removeInternal(long long, Node *, Node *);
    Node *findParent(Node *, Node *);

  public:
    BPTree();
    BPTree(int n = 3); // Constructor that sets MAX
    Address *search(long long);
    vector<Address*> searchRange(long long, long long);
    int getMaxKey(); // Method to get the parameter n of the B+ tree
    int getNumNodes(); // Method to get the number of nodes in the B+ tree
    int getNumLevels(); // Method to get the number of levels in the B+ tree
    void printRootKeys(); // Method to print the content of the root node (only the keys)
    void insert(long long, Address *);
    void remove(long long);
    void display(Node *, int);
    int countNodes(Node *);
    void displayNode(Node *);
    Node *getRoot();
};

#endif
