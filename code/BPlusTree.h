#ifndef BPTREE_H
#define BPTREE_H

#include <iostream>
#include "Address.h"
using namespace std;

// BP node
class Node {
  private:
    bool IS_LEAF;       // Boolean flag indicating whether this node is a leaf node or an internal node
    long long *key;     // Array storing the keys for this node 
    int size;           // Current number of keys in this node
    Node **ptr;         // Array of pointers to child nodes; used for internal nodes 
    Address **bptr;     // Array of pointers to data (or addresses); used for leaf nodes

  public:
    Node();             // Constructor for the Node class 

  friend class BPTree;  // BPTree class has access to private members of Node
};


// BP tree
class BPTree {
  private:
    Node *root;       // Pointer to the root node of the B+ tree
    int MAX;          // The maximum number of keys a node can hold
    int numNodes;     // Total number of nodes present in the B+ tree
    void insertInternal(long long, Node *, Node *);   // Recursive helper function to handle internal node insertions
    void removeInternal(long long, Node *, Node *);   // Recursive helper function to handle internal node deletions
    Node *findParent(Node *, Node *);                 // Helper function to find the parent of a given node

  public:
    BPTree();                   // Default constructor
    BPTree(int n = 3);          // Overloaded constructor that sets the value of MAX
    Address *search(long long); // Method to search for a key in the B+ tree and return its address
    int getMaxKey();            // Getter method for the MAX value (n of the B+ tree)
    int getNumNodes();          // Getter method for the total number of nodes in the B+ tree
    int getNumLevels();         // Method to compute the number of levels (height) in the B+ tree
    void printRootKeys();       // Utility method to print keys of the root node
    void insert(long long, Address *);  // Method to insert a key and its associated address into the B+ tree
    void remove(long long);     // Method to remove a key from the B+ tree
    void display(Node *, int);  // Utility method to display the B+ tree's structure, starting from a node
    int countNodes(Node *);     // Method to count number of nodes starting from a node
    Node *getRoot();            // Getter method to obtain the root node of the B+ tree
};

#endif
