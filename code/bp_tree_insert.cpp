// Function to insert a key-value pair into the B+ Tree.
void BPTree::insert(long long key, Block *blockPtr) { 
  // If the tree is empty.
  if (root == nullptr) {
    root = new Node;  // Create a new root node.
    root->key[0] = key;  // Assign the key value.
    root->bptr[0] = blockPtr;  // Assign the block pointer.
    root->IS_LEAF = true;  // Mark node as a leaf node.
    root->size = 1;  // Set size to 1.
  } else {  // If the tree is not empty.
    Node *currentNode = root;  // Start at the root node.
    Node *parentNode;  // Pointer to keep track of the parent node.

    // Traverse down to find the leaf node where the key should be inserted.
    while (currentNode->IS_LEAF == false) {
      parentNode = currentNode;
      for (int i = 0; i < currentNode->size; i++) {
        if (key < currentNode->key[i]) {
          currentNode = currentNode->ptr[i];
          break;
        }
        if (i == currentNode->size - 1) {
          currentNode = currentNode->ptr[i + 1];
          break;
        }
      }
    }

    // Check for duplicate keys.
    for (int i = 0; i < currentNode->size; ++i) {
      if (currentNode->key[i] == key) {
        cout << "Index already existed\n";
        return;
      }                               
    }

    // If there's room in the current leaf node.
    if (currentNode->size < MAX) {
      int i = 0;
      // Find the correct position for the new key.
      while (key > currentNode->key[i] && i < currentNode->size)
        i++;
      // Shift existing keys and block pointers to make room.
      for (int j = currentNode->size; j > i; j--) {
        currentNode->key[j] = currentNode->key[j - 1];
        currentNode->bptr[j] = currentNode->bptr[j - 1];
      }
      // Insert the new key and block pointer.
      currentNode->key[i] = key;
      currentNode->bptr[i] = blockPtr;
      currentNode->size++;  // Increment the size.
      currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size - 1];
      currentNode->ptr[currentNode->size - 1] = nullptr;
    } else {
      // Handling the case where the current node is full...
      Node *newLeaf = new Node;
      long long tempKeys[MAX + 1];
      Block *tempBlockPtrs[MAX + 1];
      for (int i = 0; i < MAX; i++) {
        tempKeys[i] = currentNode->key[i];
        tempBlockPtrs[i] = currentNode->bptr[i];
      }
      int i = 0;
      while (key > tempKeys[i] && i < MAX)
        i++;
      for (int j = MAX; j > i; j--) {
        tempKeys[j] = tempKeys[j - 1];
        tempBlockPtrs[j] = tempBlockPtrs[j - 1];
      }
      tempKeys[i] = key;
      tempBlockPtrs[i] = blockPtr;
      newLeaf->IS_LEAF = true;
      currentNode->size = (MAX + 1) / 2;
      newLeaf->size = MAX + 1 - (MAX + 1) / 2;
      currentNode->ptr[currentNode->size] = newLeaf;
      newLeaf->ptr[newLeaf->size] = currentNode->ptr[MAX];
      currentNode->ptr[MAX] = nullptr;

      for (i = 0; i < currentNode->size; i++) {
        currentNode->key[i] = tempKeys[i];
        currentNode->bptr[i] = tempBlockPtrs[i];
      }
      for (i = 0, j = currentNode->size; i < newLeaf->size; i++, j++) {
        newLeaf->key[i] = tempKeys[j];
        newLeaf->bptr[i] = tempBlockPtrs[j];
      }

      if (currentNode == root) {
        Node *newRoot = new Node;
        newRoot->key[0] = newLeaf->key[0];
        newRoot->ptr[0] = currentNode;
        newRoot->ptr[1] = newLeaf;
        newRoot->IS_LEAF = false;
        newRoot->size = 1;
        root = newRoot;
      } else {
        insertInternal(newLeaf->key[0], parentNode, newLeaf);
      }
    }
  }
}

// Function to insert a key into an internal node of the B+ Tree.
void BPTree::insertInternal(long long key, Node *currentNode, Node *childNode) {
  if (currentNode->size < MAX) {
    int i = 0;
    while (key > currentNode->key[i] && i < currentNode->size)
      i++;
    for (int j = currentNode->size; j > i; j--) {
      currentNode->key[j] = currentNode->key[j - 1];
    }
    for (int j = currentNode->size + 1; j > i + 1; j--) {
      currentNode->ptr[j] = currentNode->ptr[j - 1];
    }
    currentNode->key[i] = key;
    currentNode->size++;
    currentNode->ptr[i + 1] = childNode;
  } else {
    Node *newInternal = new Node;
    long long tempKeys[MAX + 1];
    Node *tempPtrs[MAX + 2];

    for (int i = 0; i < MAX; i++) {
      tempKeys[i] = currentNode->key[i];
    }
    for (int i = 0; i < MAX + 1; i++) {
      tempPtrs[i] = currentNode->ptr[i];
    }

    int i = 0, j;
    while (key > tempKeys[i] && i < MAX)
      i++;
    for (int j = MAX; j > i; j--) {
      tempKeys[j] = tempKeys[j - 1];
    }
    tempKeys[i] = key;
    for (int j = MAX + 1; j > i + 1; j--) {
      tempPtrs[j] = tempPtrs[j - 1];
    }
    tempPtrs[i + 1] = childNode;
    newInternal->IS_LEAF = false;
    currentNode->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;

    for (i = 0; i < currentNode->size; i++) {
      currentNode->key[i] = tempKeys[i];
    }
    for (i = 0; i < currentNode->size + 1; i++) {
      currentNode->ptr[i] = tempPtrs[i];
    }

    for (i = 0, j = currentNode->size + 1; i < newInternal->size; i++, j++) {
      newInternal->key[i] = tempKeys[j];
    }
    for (i = 0, j = currentNode->size + 1; i < newInternal->size + 1; i++, j++) {
      newInternal->ptr[i] = tempPtrs[j];
    }

    if (currentNode == root) {
      Node *newRoot = new Node;
      newRoot->key[0] = tempKeys[currentNode->size];
      newRoot->ptr[0] = currentNode;
      newRoot->ptr[1] = newInternal;
      newRoot->IS_LEAF = false;
      newRoot->size = 1;
      root = newRoot;
    } else {
      insertInternal(tempKeys[currentNode->size], findParent(root, currentNode), newInternal);
    }
  }
}
