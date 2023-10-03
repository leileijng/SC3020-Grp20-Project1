#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include "Block.h"
using namespace std;
int MAX = 3; // The optimal MAX for the dataset may be approximately 20

// BP node
class Node {
  private:
    bool IS_LEAF;
    long long *key;
    int size;
    Node **ptr;
    Block **bptr;

  public:
    Node();

  friend class BPTree;
};

// BP tree
class BPTree {
  private:
    Node *root;
    void insertInternal(long long, Node *, Node *);
    void removeInternal(long long, Node *, Node *);
    Node *findParent(Node *, Node *);

  public:
    BPTree();
    Block *search(long long);
    int getMaxKey();
    void insert(long long, Block *);
    void remove(long long);
    void display(Node *, int);
    Node *getRoot();
};
#endif