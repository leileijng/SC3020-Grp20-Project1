#include <bits/stdc++.h>
#include "StorageBlock.h"
using namespace std;
int MAX = 20;

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
    void insert(long long, Block *);
    void remove(long long);
    void display(Node *, int);
    Node *getRoot();
};
