#include <iostream>
#include "BPlusTree.h"
#include "Block.h"
// Search operation returns Block to be found
Block *BPTree::search(long long x) {              // Accepts 'long long' integer representing key value of block to search for
  if (root == NULL) {                             
    cout << "Tree is empty\n";                    // If root is 'NULL', return message informing that tree is empty
  } else {                
    Node *cursor = root;                          // Else, create Node pointer 'cursor' & initialise with root, 'cursor' indicates current point of search in tree
    while (cursor->IS_LEAF == false) {            // While cursor does not point to leaf node, continue search 
      for (int i = 0; i < cursor->size; i++) {    // Then, iterate through keys of current node
        if (x < cursor->key[i]) {                 // If x (key to search fo) is less than 
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
