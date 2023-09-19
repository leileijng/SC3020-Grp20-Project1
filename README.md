# SC3020 Project1



### 1. About Dataset

1. First clean all rows with `nan`, and save into **games_no_nan.csv**
2. We notice that only if we concate : `FG_PCT_home + TEAM_ID_home[-2:] + PTS_home + FT_PCT_home + AST_home + REB_home + HOME_TEAM_WINS` as the index feature that we can differentiate all rows (except some repeated rows).
3. The final dataset is stored into **games_final.csv** 



### 2. About B+ tree

```
g++ BPlusTree.cpp StorageBlock.cpp
```

```
./a.out
```

The b+tree structure (insert/delete) can refer https://www.programiz.com/dsa/b-plus-tree.

Beyond that, for each leaf node, we maintain `bptr` which points to a Block storing all information about one row in dataset.

The use of functions can refer the `main` function in BPlusTree.cpp.



### 3. What to do next

1. import all data from the dataset and build the B+tree through iteratively insert.

2. evaluate the B+tree, such as the depth. (can update the value of `MAX` in BPlusTree.h for fine tune)

3. write the `search_node_ptr(x)` func, very similar as `search` func except the return variable.

4. find the number of a prefix of leaf nodes ending up at $x$.

   1. first `search_node_ptr(1000..000)` to get the smallest leaf node
   2. iteratively find next linked nodes utill finding the $x$

   ```
   func find_prefix(x){
   	leaf_ptr = search_node_ptr(1000.000); // smallest number
   	int count = 0;
   	while(true){
   		int size = leaf_ptr->size;
   		for(int i = 0; i < size; ++ i){
   			if(leaf_ptr->key[i] == x){
   				// founded
   				return count;
   			}
   			count ++;
   		}
   		leaf_ptr = leaf_ptr->ptr[size];
   	}
   }
   ```

5. find the number of suffix of leaf nodes starting from $x$.

   1. first `search_node_ptr(x)` to get the leaf node containing $x$
   2. iteratively find next linked nodes utill finding the end

   ```
   func find_suffix(x){
   	leaf_ptr = search_node_ptr(x) ;
   	int count = 0;
   	int size = leaf_ptr->size;
     for(int i = 0; i < size; ++ i){
       if(leaf_ptr->key[i] >= x){
       	count ++;
       }
     }
     leaf_ptr = leaf_ptr->ptr[size];
   	while(leaf_ptr != NULL){
   		int size = leaf_ptr->size;
   		count += size;
   		leaf_ptr = leaf_ptr->ptr[size];
   	}
   	return count;
   }
   ```

6. reframe the code, making it clearer and standard



