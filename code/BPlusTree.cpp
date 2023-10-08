#include "BPlusTree.h"
#include <iostream>
#include "Address.h"
#include "Record.h"
#include <cmath>
#include <chrono> 
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

int MAX = 20; // The optimal MAX for the dataset may be approximately 20
Node::Node() {
  key = new long long[MAX + 5];
  ptr = new Node *[MAX + 5];
  bptr = new Address *[MAX + 5];
}

// Existing BPTree constructor
BPTree::BPTree() {
  root = NULL;
  MAX = 20; // Initialize MAX
  numNodes = 0; // Initialize numNodes to 0
}

// New constructor that sets MAX
BPTree::BPTree(int n) {
  root = NULL;
  MAX = n; // Initialize MAX to n
  numNodes = 0; // Initialize numNodes to 0
}

int BPTree::getMaxKey(){
  return MAX;
}

// get the number of nodes in the B+ tree
int BPTree::getNumNodes() {
  this->numNodes = countNodes(root);
  return this->numNodes;
}

int BPTree::getNumLevels() {
  if (root == NULL) return 0;
  int levels = 0;
  Node *cursor = root;
  while (!cursor->IS_LEAF) {
    levels++;
    cursor = cursor->ptr[0];
  }
  return levels + 1; // Add 1 for the leaf level
}

void BPTree::printRootKeys() {
  if (root == NULL) {
    cout << "Tree is empty." << endl;
    return;
  }
  cout << "Keys in the root node: ";
  for (int i = 0; i < root->size; i++) {
    cout << root->key[i] << " ";
  }
  cout << endl;
}

long long BPTree::findMaxKey(){
  Node* cursor = root;
  while(cursor->IS_LEAF != true){
    cursor = cursor->ptr[cursor->size];
  }
  return cursor->key[(cursor->size)-1];
}


Address *BPTree::search(long long x) {
  if (root == NULL) {
    cout << "Tree is empty\n";
  } else {
    Node *cursor = root;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    for (int i = 0; i < cursor->size; i++) {
      if (cursor->key[i] == x) {
        cout << "Found\n";
        return cursor->bptr[i];
      }
    }
    cout << "Not found\n";
  }
  return NULL;
}

    // Function to extract the first 3 digits of a long long number
long long getFirst3Digits(long long num) {
    int length = to_string(num).length();
    return num / pow(10, length - 3);
}

vector<Address*> BPTree::searchExact(long long x, int &leafNodeCount, int &nonLeafNodeCount)  {
    vector<Address*> result;
    if (root == NULL) {
        cout << "Tree is empty\n";
        return result;
    }
    // Start from the root
    Node *cursor = root;

    leafNodeCount = 0;
    nonLeafNodeCount = 0;

    // Traverse down to the leaf level
    while (cursor->IS_LEAF == false) {
      // Increment index node counter
      nonLeafNodeCount++;
      for (int i = 0; i < cursor->size; i++) {
          if (x < cursor->key[i]) {
            cursor = cursor->ptr[i];
            break;
          }
          if (i == cursor->size - 1) {
            cursor = cursor->ptr[i + 1];
            break;
          }
      }
    }

    long long first3DigitsOfX = getFirst3Digits(x);
    // cout << x << endl;

    // int tempcnt = 0;

    // Now traverse the leaf nodes to collect the keys that match the first 3 digits
    while (cursor != nullptr) {
      if(getFirst3Digits(cursor->key[0]) > first3DigitsOfX) break;
      leafNodeCount++;

      //cout<<"new node: "<<endl;
        for (int i = 0; i < cursor->size; i++) {
          long long first3DigitsOfKey = getFirst3Digits(cursor->key[i]);
          if (first3DigitsOfKey == first3DigitsOfX) {
            result.push_back(cursor->bptr[i]);
          }
          // if(tempcnt < 30) {
          //   cout<<cursor->key[i]<<endl;
          //   tempcnt += 1;
          // }
          // else return result;
        }
        cursor = cursor->ptr[cursor->size];  // Move to the next leaf node
    }
    // leafNodeCount = 142;
    return result;
}

vector<Address*> BPTree::searchRange(long long x, long long y, int &nodesAccessed, int &nonLeafNodeCount) {
    vector<Address*> result;
    nodesAccessed = 0;  // Initialize the counter to zero
    nonLeafNodeCount = 0;

    if (root == NULL) {
        cout << "Tree is empty\n";
        return result;
    }

    long long maxKey = findMaxKey();

    // Start from the root
    Node *cursor = root;
    nodesAccessed++;  // Increment the counter as we've accessed the root

    // Traverse down to the leaf level
    while (cursor->IS_LEAF == false) {
        for (int i = 0; i < cursor->size; i++) {
            if (x < cursor->key[i]) {
                nodesAccessed++;
                cursor = cursor->ptr[i];
                break;
            }
            if (i == cursor->size - 1) {
                nodesAccessed++;
                cursor = cursor->ptr[i + 1];
                break;
            }
        }
    }

    // Now traverse the leaf nodes to collect the keys in the range [x, y]
    while (cursor != nullptr) {
        if(cursor->key[0] > y) break;
        nonLeafNodeCount++;  // Increment the counter as we're about to access a new node
        for (int i = 0; i < cursor->size; i++) {
            if (cursor->key[i] >= x && cursor->key[i] <= y) {
                result.push_back(cursor->bptr[i]);
            }
            // Since the keys are sorted, no point in checking further if we've crossed y
            if (cursor->key[i] == maxKey) {
                return result;
            }
        }
        cursor = cursor->ptr[cursor->size];  // Move to the next leaf node
    }
    return result;
}


// Insertion Operation for B+ tree
void BPTree::insert(long long x, Address *bptr) {

  // If tree is empty, initialise root with new key-pointer (insert here)
  if (root == NULL) {
    root = new Node;
    root->key[0] = x;
    root->bptr[0] = bptr;
    root->IS_LEAF = true;
    root->size = 1;
  } 
  
  // Else, locate correct leaf node where new key-pointer should be inserted
  else {
    Node *cursor = root;                        // Start at root node & traverse down
    Node *parent;                               // Keep track of parent node as we traverse
    while (cursor->IS_LEAF == false) {          // Keep traversing until leaf node reached
      parent = cursor;                          // Store current node as parent before going one level deeper
      for (int i = 0; i < cursor->size; i++) {  // Iterate over all keys in current node
        if (x < cursor->key[i]) {               // If new key 'x' less than current key,
          cursor = cursor->ptr[i];              // Go to child pointer on left of this key
          break;                  
        }
        if (i == cursor->size - 1) {            // If reached last key in current node & haven't found smaller key
          cursor = cursor->ptr[i + 1];          // Go to child pointer on right of this key
          break;
        }
      }
    }

    // At this point, cursor should be pointing to leaf node that key 'x' should be inserted
    // Iterate through node and check for existence of key
    for (int i = 0; i < cursor->size; ++ i){
      if(cursor->key[i] == x){   // If key exists, return
        return ;
      }
    }

    // If current leaf node has room for new key to be inserted
    if (cursor->size < MAX) {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size) // Find position where new key should be inserted
        i++;
      for (int j = cursor->size; j > i; j--) {       // Shift keys to right of insertion point to make space for insertion
        cursor->key[j] = cursor->key[j - 1];
        cursor->bptr[j] = cursor->bptr[j - 1];
      }
      cursor->key[i] = x;                            // Insert new key & pointer
      cursor->bptr[i] = bptr;
      cursor->size++;                                // Increase size of node
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];  // Adjust rightmost pointer
      cursor->ptr[cursor->size - 1] = NULL;
    } 
    
    // Else if current leaf node is full, it needs to be split to accomodate new key
    else {
      Node *newLeaf = new Node;                     // Creating new node for overflowed keys
      long long virtualNode[MAX + 5];
      Address *tmpBptr[MAX + 5];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];            // Arrays to hold keys temporarily 
        tmpBptr[i] = cursor->bptr[i];
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < MAX)         // Finding position to insert key
        i++;
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];        // Shift keys to right of insertion point
        tmpBptr[j] = tmpBptr[j - 1];
      }
      virtualNode[i] = x;                           // Insert new key & pointer
      tmpBptr[i] = bptr;
      newLeaf->IS_LEAF = true;              
      cursor->size = (MAX + 1) / 2;                // Redefining size after split
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;         // Rightmost pointer of existing node points to new leaf pointer
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX]; // Rightmost pointer of leaf node points to right neighbouring node
      cursor->ptr[MAX] = NULL;                     // This pointer is no longer rightmost pointer
      
      // Copy new key-pointers to split node
      for (i = 0; i < cursor->size; i++) {         
        cursor->key[i] = virtualNode[i];
        cursor->bptr[i] = tmpBptr[i];
      }

      // Copy new key-pointers to new leaf node
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) { 
        newLeaf->key[i] = virtualNode[j];
        newLeaf->bptr[i] = tmpBptr[j];
      }

      // If split node was root node, we will require new root node
      if (cursor == root) {                       
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->bptr[0] = newLeaf->bptr[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;

      } 
      
      // Else, we will insert a key to internal node (which might warrant further splitting)
      else {
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}

// Insert Key to Internal Node
void BPTree::insertInternal(long long x, Node *cursor, Node *child) {

  // If internal node not full
  if (cursor->size < MAX) {
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)    // Find position to insert
      i++;
    for (int j = cursor->size; j > i; j--) {          // Shift keys to right of insertion position
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {  // Shift pointers to right of insertion position
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;                               // Insert key
    cursor->size++;                                   // Update size
    cursor->ptr[i + 1] = child;                       // Set child pointer
  } 
  
  // Else, split internal node
  else {
    Node *newInternal = new Node;                     // Create new node for overflowed keys
    long long virtualKey[MAX + 5];
    Node *virtualPtr[MAX + 5];
    for (int i = 0; i < MAX; i++) {                   // Store keys in temporary array
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {               // Store pointers in temporary array
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)              // Find position to insert
      i++;
    for (int j = MAX + 1; j > i; j--) {               // Shift keys to right of insertion position
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--) {           // Shift pointers to right of insertion position
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;                        // Set child pointer
    newInternal->IS_LEAF = false;                     // New internal node is not lead node
    cursor->size = (MAX + 1) / 2;                     // Redefine size after split
    newInternal->size = MAX - (MAX + 1) / 2;

    for (i = 0; i < cursor->size; i++) {              // Copy keys to split node
      cursor->key[i] = virtualKey[i];
    }
    for (i = 0; i < cursor->size + 1; i++) {          // Copy pointers to split node
      cursor->ptr[i] = virtualPtr[i];
    }

    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {     // Copy keys to new node
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) { // Copy pointers to new node
      newInternal->ptr[i] = virtualPtr[j];
    }

    // If split node is root, insert new rode
    if (cursor == root) {                            
      Node *newRoot = new Node;
      newRoot->key[0] = virtualKey[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;

    } 
    
    // Else, insert new internal node
    else {                                     
      insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
  }
}

// Remove
void BPTree::remove(long long x) {
  if (root == NULL) {
    cout << "Tree empty\n";
  } else {
    Node *cursor = root;
    Node *parent;
    int leftSibling, rightSibling;
    while (cursor->IS_LEAF == false) {
      for (int i = 0; i < cursor->size; i++) {
        parent = cursor;
        leftSibling = i - 1;
        rightSibling = i + 1;
        if (x < cursor->key[i]) {
          cursor = cursor->ptr[i];
          break;
        }
        if (i == cursor->size - 1) {
          leftSibling = i;
          rightSibling = i + 2;
          cursor = cursor->ptr[i + 1];
          break;
        }
      }
    }
    bool found = false;
    int pos;
    for (pos = 0; pos < cursor->size; pos++) {
      if (cursor->key[pos] == x) {
        found = true;
        break;
      }
    }
    if (!found) {
      //cout << "1 Not found\n";
      return;
    }
    for (int i = pos; i < cursor->size; i++) {
      cursor->key[i] = cursor->key[i + 1];
      cursor->bptr[i] = cursor->bptr[i + 1];
    }
    cursor->size--;
    if (cursor == root) {
      for (int i = 0; i < MAX + 1; i++) {
        cursor->ptr[i] = NULL;
      }
      if (cursor->size == 0) {
        cout << "Tree died\n";
        delete[] cursor->key;
        delete[] cursor->bptr;
        delete[] cursor->ptr;
        delete cursor;
        root = NULL;
      }
      return;
    }
    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = NULL;
    if (cursor->size >= (MAX + 1) / 2) {
      return;
    }
    if (leftSibling >= 0) {
      Node *leftNode = parent->ptr[leftSibling];
      if (leftNode->size >= (MAX + 1) / 2 + 1) {
        for (int i = cursor->size; i > 0; i--) {
          cursor->key[i] = cursor->key[i - 1];
          cursor->bptr[i] = cursor->bptr[i - 1];
        }
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        cursor->key[0] = leftNode->key[leftNode->size - 1];
        cursor->bptr[0] = leftNode->bptr[leftNode->size - 1];
        leftNode->size--;
        leftNode->ptr[leftNode->size] = cursor;
        leftNode->ptr[leftNode->size + 1] = NULL;
        parent->key[leftSibling] = cursor->key[0];
        return;
      }
    }
    if (rightSibling <= parent->size) {
      Node *rightNode = parent->ptr[rightSibling];
      if (rightNode->size >= (MAX + 1) / 2 + 1) {
        cursor->size++;
        cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
        cursor->ptr[cursor->size - 1] = NULL;
        cursor->key[cursor->size - 1] = rightNode->key[0];
        cursor->bptr[cursor->size - 1] = rightNode->bptr[0];
        rightNode->size--;
        rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
        rightNode->ptr[rightNode->size + 1] = NULL;
        for (int i = 0; i < rightNode->size; i++) {
          rightNode->key[i] = rightNode->key[i + 1];
          rightNode->bptr[i] = rightNode->bptr[i + 1];
        }
        parent->key[rightSibling - 1] = rightNode->key[0];
        return;
      }
    }
    if (leftSibling >= 0) {
      Node *leftNode = parent->ptr[leftSibling];
      for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
        leftNode->key[i] = cursor->key[j];
        leftNode->bptr[i] = cursor->bptr[j];
      }
      leftNode->ptr[leftNode->size] = NULL;
      leftNode->size += cursor->size;
      leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
      removeInternal(parent->key[leftSibling], parent, cursor);
      delete[] cursor->key;
      delete[] cursor->bptr;
      delete[] cursor->ptr;
      delete cursor;
    } else if (rightSibling <= parent->size) {
      Node *rightNode = parent->ptr[rightSibling];
      for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
        cursor->key[i] = rightNode->key[j];
        cursor->bptr[i] = rightNode->bptr[j];
      }
      cursor->ptr[cursor->size] = NULL;
      cursor->size += rightNode->size;
      cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
      //cout << "Merging two leaf nodes\n";
      removeInternal(parent->key[rightSibling - 1], parent, rightNode);
      delete[] rightNode->key;
      delete[] rightNode->bptr;
      delete[] rightNode->ptr;
      delete rightNode;
    }
  }
}

// Remove Internal
void BPTree::removeInternal(long long x, Node *cursor, Node *child) {
  if (cursor == root) {
    if (cursor->size == 1) {
      if (cursor->ptr[1] == child) {
        // delete[] child->key;
        // delete[] child->bptr;
        // delete[] child->ptr;
        // delete child;
        root = cursor->ptr[0];
        delete[] cursor->key;
        delete[] cursor->bptr;
        delete[] cursor->ptr;
        delete cursor;
        cout << "Changed root node\n";
        return;
      } else if (cursor->ptr[0] == child) {
        // delete[] child->key;
        // delete[] child->bptr;
        // delete[] child->ptr;
        // delete child;
        root = cursor->ptr[1];
        delete[] cursor->key;
        delete[] cursor->bptr;
        delete[] cursor->ptr;
        delete cursor;
        cout << "Changed root node\n";
        return;
      }
    }
  }
  int pos;
  for (pos = 0; pos < cursor->size; pos++) {
    if (cursor->key[pos] == x) {
      break;
    }
  }
  for (int i = pos; i < cursor->size; i++) {
    cursor->key[i] = cursor->key[i + 1];
    cursor->bptr[i] = cursor->bptr[i + 1];
  }
  for (pos = 0; pos < cursor->size + 1; pos++) {
    if (cursor->ptr[pos] == child) {
      break;
    }
  }
  for (int i = pos; i < cursor->size + 1; i++) {
    cursor->ptr[i] = cursor->ptr[i + 1];
  }
  cursor->size--;
  if (cursor->size >= (MAX + 1) / 2 - 1) {
    return;
  }
  if (cursor == root)
    return;
  Node *parent = findParent(root, cursor);
  int leftSibling = 0, rightSibling = 0;
  for (pos = 0; pos < parent->size + 1; pos++) {
    if (parent->ptr[pos] == cursor) {
      leftSibling = pos - 1;
      rightSibling = pos + 1;
      break;
    }
  }
  if (leftSibling >= 0) {
    Node *leftNode = parent->ptr[leftSibling];
    if (leftNode->size >= (MAX + 1) / 2) {
      for (int i = cursor->size; i > 0; i--) {
        cursor->key[i] = cursor->key[i - 1];
        cursor->bptr[i] = cursor->bptr[i - 1];
      }
      cursor->key[0] = parent->key[leftSibling];
      cursor->bptr[0] = parent->bptr[leftSibling];
      parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
      parent->bptr[leftSibling] = leftNode->bptr[leftNode->size - 1];
      for (int i = cursor->size + 1; i > 0; i--) {
        cursor->ptr[i] = cursor->ptr[i - 1];
      }
      cursor->ptr[0] = leftNode->ptr[leftNode->size];
      cursor->size++;
      leftNode->size--;
      return;
    }
  }
  if (rightSibling <= parent->size) {
    Node *rightNode = parent->ptr[rightSibling];
    if (rightNode->size >= (MAX + 1) / 2) {
      cursor->key[cursor->size] = parent->key[pos];
      cursor->bptr[cursor->size] = parent->bptr[pos];
      parent->key[pos] = rightNode->key[0];
      parent->bptr[pos] = rightNode->bptr[0];
      for (int i = 0; i < rightNode->size - 1; i++) {
        rightNode->key[i] = rightNode->key[i + 1];
        rightNode->bptr[i] = rightNode->bptr[i + 1];
      }
      cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
      for (int i = 0; i < rightNode->size; ++i) {
        rightNode->ptr[i] = rightNode->ptr[i + 1];
      }
      cursor->size++;
      rightNode->size--;
      return;
    }
  }
  if (leftSibling >= 0) {
    Node *leftNode = parent->ptr[leftSibling];
    leftNode->key[leftNode->size] = parent->key[leftSibling];
    leftNode->bptr[leftNode->size] = parent->bptr[leftSibling];
    for (int i = leftNode->size + 1, j = 0; j < cursor->size; i++, j++) {
      leftNode->key[i] = cursor->key[j];
      leftNode->bptr[i] = cursor->bptr[j];
    }
    for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; i++, j++) {
      leftNode->ptr[i] = cursor->ptr[j];
      cursor->ptr[j] = NULL;
    }
    leftNode->size += cursor->size + 1;
    cursor->size = 0;
    removeInternal(parent->key[leftSibling], parent, cursor);
  } else if (rightSibling <= parent->size) {
    Node *rightNode = parent->ptr[rightSibling];
    cursor->key[cursor->size] = parent->key[rightSibling - 1];
    cursor->bptr[cursor->size] = parent->bptr[rightSibling - 1];
    for (int i = cursor->size + 1, j = 0; j < rightNode->size; i++, j++) {
      cursor->key[i] = rightNode->key[j];
      cursor->bptr[i] = rightNode->bptr[j];
    }
    for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; i++, j++) {
      cursor->ptr[i] = rightNode->ptr[j];
      rightNode->ptr[j] = NULL;
    }
    cursor->size += rightNode->size + 1;
    rightNode->size = 0;
    removeInternal(parent->key[rightSibling - 1], parent, rightNode);
  }
}

vector<Address*> BPTree::removeKeysBelow(long long x) {
  vector <Address*> result;

  if (root == NULL) {
    cout << "Tree empty\n";
    return result; // No records deleted
  }

  // Start from the root
  Node *cursor = root;
  // Traverse down to the leaf level to find the smallest key
  while (cursor->IS_LEAF == false) {
    cursor = cursor->ptr[0];
  }

  vector <long long> removeKeys;
  bool finded = false;
  while(true){
    for(int i = 0; i < cursor->size; ++ i){
      if(getFirst3Digits(cursor->key[i]) <= getFirst3Digits(x)){
        removeKeys.push_back(cursor->key[i]);
        result.push_back(cursor->bptr[i]);
      } else {
        finded = true;
        break;
      }
    }
    if(finded) break;
    else {
      cursor = cursor->ptr[cursor->size];
    }
  }

  for(int i = 0; i < removeKeys.size(); ++ i){
    remove(removeKeys[i]);
  }
  return result;
  
  // // Now traverse the leaf nodes to remove keys below x
  // while (cursor != nullptr) {
  //   bool removed = false;
  //   for (int i = 0; i < cursor->size; ) {
  //     if (cursor->key[i] < x) {
  //       cout<<"count"<<count<<endl;
  //       long long keyToRemove = cursor->key[i];
  //       cout<<"delete index: "<<keyToRemove<<endl;
  //       // Remove this key and handle underflow
  //       remove(keyToRemove);
  //       removed = true;
  //       count++; // Increment the counter
  //     } else {
  //       i++;
  //     }
  //   }

  //   if (removed) {
  //     // If keys were removed, cursor might have been deleted or changed.
  //     // So, we reset cursor to the beginning to continue the operation.
  //     cursor = root;
  //     while (cursor->IS_LEAF == false) {
  //       cursor = cursor->ptr[0];
  //     }
  //   } else {
  //     cursor = cursor->ptr[cursor->size];  // Move to the next leaf node
  //   }
  // }
}



// Find the parent
Node *BPTree::findParent(Node *cursor, Node *child) {
  Node *parent;
  if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
    return NULL;
  }
  for (int i = 0; i < cursor->size + 1; i++) {
    if (cursor->ptr[i] == child) {
      parent = cursor;
      return parent;
    } else {
      parent = findParent(cursor->ptr[i], child);
      if (parent != NULL)
        return parent;
    }
  }
  return parent;
}

// Count the number of tree nodes
int BPTree::countNodes(Node *cursor){
  if (cursor != NULL){
    int count = 1;
    if(cursor->IS_LEAF != true) {
      for(int i = 0; i < cursor->size + 1; i++) {
        count += countNodes(cursor->ptr[i]);
      }
    }
    return count;
  }
  return 0;
}

// Print the tree
void BPTree::display(Node *cursor, int depth) {
  if (cursor != NULL) {
    cout << "( " << depth << " ) : ";
    for (int i = 0; i < cursor->size; i++) {
      cout << cursor->key[i] << " ";
    }
    cout << "\n";
    if (cursor->IS_LEAF != true) {
      for (int i = 0; i < cursor->size + 1; i++) {
        display(cursor->ptr[i], depth + 1);
      }
    }
  }
  if (cursor == root) puts("-------------------");
}


void BPTree::displayNode(Node *node) {
    if (node == nullptr) {
        cout << "Node is null." << endl;
        return;
    }

    const int maxCellWidth = 20;  // Adjust as needed
    const int minCellWidth = 10;  // Minimum width for null or empty cells

    cout << "----------------------------- Node Start -----------------------------" << endl;

    if (node->IS_LEAF) {
        // Display leaf node
        cout << "| Type: Leaf" << endl;
        cout << "|---------------------------------------------------------------------|" << endl;
        cout << "| Address | Key       |" << endl;
        cout << "|---------------------------------------------------------------------|" << endl;
        cout << "|";
        for (int i = 0; i < MAX + 5; ++i) {
            int cellWidth = (node->bptr[i] && node->key[i]) ? maxCellWidth : minCellWidth;
            cout << setw(cellWidth) << (node->bptr[i] ? "Address" : "null");
            cout << setw(cellWidth) << (node->key[i] ? to_string(node->key[i]) : "null");
            if (i < MAX + 4) {
                cout << "|";
            }
        }
        cout << endl;
        // Display next link if exists
        if (node->ptr[MAX + 4] != nullptr) {
            cout << "|---------------------------------------------------------------------|" << endl;
            cout << "| Next Link: Exists" << endl;
        }
    } else {
        // Display internal node
        cout << "| Type: Internal" << endl;
        cout << "|---------------------------------------------------------------------|" << endl;
        cout << "| Node   | Key       |" << endl;
        cout << "|---------------------------------------------------------------------|" << endl;
        cout << "|";
        for (int i = 0; i < MAX + 5; ++i) {
            int cellWidth = (node->ptr[i] && node->key[i]) ? maxCellWidth : minCellWidth;
            cout << setw(cellWidth) << (node->ptr[i] ? "Node" : "null");
            cout << setw(cellWidth) << (node->key[i] ? to_string(node->key[i]) : "null");
            if (i < MAX + 4) {
                cout << "|";
            }
        }
        cout << endl;
    }

    cout << "------------------------------ Node End ------------------------------" << endl;
}

void BPTree::travel(Node *cursor) {
  if(cursor == NULL) return ;
  long long maxKey = findMaxKey();
  cout << "maxKey: " << maxKey << endl;
  while(cursor->IS_LEAF != true) cursor = cursor->ptr[0];
  int count = 0;
  while(cursor != NULL){
    cout << cursor->key[0] << " " << cursor->size << endl;
    for(int i = 0; i < cursor->size; ++ i){
      printf("%lld ", cursor->key[i]);
      if(cursor->key[i] == maxKey) return;
    }
    puts("");
    cursor = cursor->ptr[cursor->size];
  }
  puts("hhh");
}

// Get the root
Node *BPTree::getRoot() {
  return root;
}
/*
int main() {
  BPTree tree;
  Address *bptr1 = new Address("22/12/2022", "1610612740", 126, 0.484, 0.926, 0.382, 25, 46, 1);
  Address *bptr2 = new Address("22/12/2022", "1610612762", 120, 0.488, 0.952, 0.457, 16, 40, 1);
  Address *bptr3 = new Address("21/12/2022", "1610612739", 114, 0.482, 0.786, 0.313, 22, 37, 1);
  Address *bptr4 = new Address("21/12/2022", "1610612755", 113, 0.441, 0.909, 0.297, 27, 49, 1);
  Address *bptr5 = new Address("21/12/2022", "1610612737", 108, 0.429, 1, 0.378, 22, 47, 0);
  Address *bptr6 = new Address("21/12/2022", "1610612738", 112, 0.386, 0.84, 0.317, 26, 62, 0);
  Address *bptr7 = new Address("21/12/2022", "1610612751", 143, 0.643, 0.875, 0.636, 42, 32, 1);
  Address *bptr8 = new Address("21/12/2022", "1610612752", 106, 0.553, 0.611, 0.423, 25, 38, 0);
  Address *bptr9 = new Address("21/12/2022", "1610612745", 110, 0.466, 0.647, 0.395, 22, 49, 0);
  Address *bptr10 = new Address("21/12/2022", "1610612750", 99, 0.494, 0.7, 0.267, 23, 39, 0);
  Address *bptr11 = new Address("21/12/2022", "1610612760", 101, 0.468, 0.84, 0.333, 19, 37, 1);

  tree.insert(48440126092625400, bptr1); // SEQ_NUMBER
  tree.insert(48862120095216400, bptr2);
  tree.insert(48239114078622300, bptr3);
  tree.insert(44155113090927400, bptr4);
  tree.insert(42937108100022400, bptr5);
  tree.insert(38638112084026600, bptr6);
  tree.insert(64351143087542300, bptr7);
  tree.insert(55352106061125300, bptr8);
  tree.insert(46645110064722400, bptr9);
  tree.insert(49450099070023300, bptr10);
  tree.insert(46860101084019300, bptr11);
  
  tree.display(tree.getRoot(), 1);

  tree.remove(48440126092625400);
  tree.display(tree.getRoot(), 1);

  tree.remove(46860101084019300);
  tree.display(tree.getRoot(), 1);

  tree.remove(55352106061125300);
  tree.display(tree.getRoot(), 1);

  Address *bptr = tree.search(38638112084026600);
  cout<<bptr<<"here"<<endl;
  if(bptr != NULL) bptr->displayStats();
  return 0;
}*/