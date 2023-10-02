// B+ Tree Structure Reference: https://www.programiz.com/dsa/b-plus-tree
// Header files
#include <bits/stdc++.h>
#include "BPlusTree.h"
using namespace std;

// Constructor for Node class
Node::Node() {
  key = new long long[MAX + 5];  // Array of 'long long' integers holding keys of the node
  ptr = new Node *[MAX + 5];     // Array of 'Node' pointers holding pointers to child nodes
  bptr = new Block *[MAX + 5];   // Array of 'Block' pointers maintained for each leaf node,                                  
}                                // all of which point to a Block storing all information about one row in dataset

// Constructor for BPTree class 
BPTree::BPTree() {
  root = NULL;                  // Initialise 'root' pointer to 'NULL', indicating that tree is empty at the time of creation
}
