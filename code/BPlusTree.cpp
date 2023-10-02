#include "BPlusTree.h"
#include <iostream>
using namespace std;

Node::Node() {
  key = new long long[MAX + 5];
  ptr = new Node *[MAX + 5];
  bptr = new Block *[MAX + 5];
}

BPTree::BPTree() {
  root = NULL;
}

// Search operation
Block *BPTree::search(long long x) {
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

// Insert
void BPTree::insert(long long x, Block *bptr) {
  if (root == NULL) {
    root = new Node;
    root->key[0] = x;
    root->bptr[0] = bptr;
    root->IS_LEAF = true;
    root->size = 1;
  } else {
    Node *cursor = root;
    Node *parent;
    while (cursor->IS_LEAF == false) {
      parent = cursor;
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
    for (int i = 0; i < cursor->size; ++ i){
      if(cursor->key[i] == x){
        cout << "Index already existed\n";
        return ;
      }
    }
    if (cursor->size < MAX) {
      int i = 0;
      while (x > cursor->key[i] && i < cursor->size)
        i++;
      for (int j = cursor->size; j > i; j--) {
        cursor->key[j] = cursor->key[j - 1];
        cursor->bptr[j] = cursor->bptr[j - 1];
      }
      cursor->key[i] = x;
      cursor->bptr[i] = bptr;
      cursor->size++;
      cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
      cursor->ptr[cursor->size - 1] = NULL;
    } else {
      Node *newLeaf = new Node;
      long long virtualNode[MAX + 5];
      Block *tmpBptr[MAX + 5];
      for (int i = 0; i < MAX; i++) {
        virtualNode[i] = cursor->key[i];
        tmpBptr[i] = cursor->bptr[i];
      }
      int i = 0, j;
      while (x > virtualNode[i] && i < MAX)
        i++;
      for (int j = MAX + 1; j > i; j--) {
        virtualNode[j] = virtualNode[j - 1];
        tmpBptr[j] = tmpBptr[j - 1];
      }
      virtualNode[i] = x;
      tmpBptr[i] = bptr;
      newLeaf->IS_LEAF = true;
      cursor->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      cursor->ptr[cursor->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];
      cursor->ptr[MAX] = NULL;
      for (i = 0; i < cursor->size; i++) {
        cursor->key[i] = virtualNode[i];
        cursor->bptr[i] = tmpBptr[i];
      }
      for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = virtualNode[j];
        newLeaf->bptr[i] = tmpBptr[j];
      }
      if (cursor == root) {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->bptr[0] = newLeaf->bptr[0];
        newRoot->ptr[0] = cursor;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->key[0], parent, newLeaf);
      }
    }
  }
}

// Insert Internal
void BPTree::insertInternal(long long x, Node *cursor, Node *child) {
  if (cursor->size < MAX) {
    int i = 0;
    while (x > cursor->key[i] && i < cursor->size)
      i++;
    for (int j = cursor->size; j > i; j--) {
      cursor->key[j] = cursor->key[j - 1];
    }
    for (int j = cursor->size + 1; j > i + 1; j--) {
      cursor->ptr[j] = cursor->ptr[j - 1];
    }
    cursor->key[i] = x;
    cursor->size++;
    cursor->ptr[i + 1] = child;
  } else {
    Node *newInternal = new Node;
    long long virtualKey[MAX + 5];
    Node *virtualPtr[MAX + 5];
    for (int i = 0; i < MAX; i++) {
      virtualKey[i] = cursor->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      virtualPtr[i] = cursor->ptr[i];
    }
    int i = 0, j;
    while (x > virtualKey[i] && i < MAX)
      i++;
    for (int j = MAX + 1; j > i; j--) {
      virtualKey[j] = virtualKey[j - 1];
    }
    virtualKey[i] = x;
    for (int j = MAX + 2; j > i + 1; j--) {
      virtualPtr[j] = virtualPtr[j - 1];
    }
    virtualPtr[i + 1] = child;
    newInternal->IS_LEAF = false;
    cursor->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;

    for (i = 0; i < cursor->size; i++) {
      cursor->key[i] = virtualKey[i];
    }
    for (i = 0; i < cursor->size + 1; i++) {
      cursor->ptr[i] = virtualPtr[i];
    }

    for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = virtualKey[j];
    }
    for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = virtualPtr[j];
    }
    if (cursor == root) {
      Node *newRoot = new Node;
      newRoot->key[0] = virtualKey[cursor->size];
      newRoot->ptr[0] = cursor;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
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
      cout << "Not found\n";
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
      cout << "Merging two leaf nodes\n";
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
        delete[] child->key;
        delete[] child->bptr;
        delete[] child->ptr;
        delete child;
        root = cursor->ptr[0];
        delete[] cursor->key;
        delete[] cursor->bptr;
        delete[] cursor->ptr;
        delete cursor;
        cout << "Changed root node\n";
        return;
      } else if (cursor->ptr[0] == child) {
        delete[] child->key;
        delete[] child->bptr;
        delete[] child->ptr;
        delete child;
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
  int leftSibling, rightSibling;
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
    for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++) {
      leftNode->key[i] = cursor->key[j];
      leftNode->bptr[i] = cursor->bptr[j];
    }
    for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++) {
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
    for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++) {
      cursor->key[i] = rightNode->key[j];
      cursor->bptr[i] = rightNode->bptr[j];
    }
    for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++) {
      cursor->ptr[i] = rightNode->ptr[j];
      rightNode->ptr[j] = NULL;
    }
    cursor->size += rightNode->size + 1;
    rightNode->size = 0;
    removeInternal(parent->key[rightSibling - 1], parent, rightNode);
  }
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

// Get the root
Node *BPTree::getRoot() {
  return root;
}

int main() {
  BPTree tree;
  Block *bptr1 = new Block("22/12/2022", "1610612740", 126, 0.484, 0.926, 0.382, 25, 46, 1);
  Block *bptr2 = new Block("22/12/2022", "1610612762", 120, 0.488, 0.952, 0.457, 16, 40, 1);
  Block *bptr3 = new Block("21/12/2022", "1610612739", 114, 0.482, 0.786, 0.313, 22, 37, 1);
  Block *bptr4 = new Block("21/12/2022", "1610612755", 113, 0.441, 0.909, 0.297, 27, 49, 1);
  Block *bptr5 = new Block("21/12/2022", "1610612737", 108, 0.429, 1, 0.378, 22, 47, 0);
  Block *bptr6 = new Block("21/12/2022", "1610612738", 112, 0.386, 0.84, 0.317, 26, 62, 0);
  Block *bptr7 = new Block("21/12/2022", "1610612751", 143, 0.643, 0.875, 0.636, 42, 32, 1);
  Block *bptr8 = new Block("21/12/2022", "1610612752", 106, 0.553, 0.611, 0.423, 25, 38, 0);
  Block *bptr9 = new Block("21/12/2022", "1610612745", 110, 0.466, 0.647, 0.395, 22, 49, 0);
  Block *bptr10 = new Block("21/12/2022", "1610612750", 99, 0.494, 0.7, 0.267, 23, 39, 0);
  Block *bptr11 = new Block("21/12/2022", "1610612760", 101, 0.468, 0.84, 0.333, 19, 37, 1);

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

  Block *bptr = tree.search(38638112084026600);
  if(bptr != NULL) bptr->displayStats();
  return 0;
}