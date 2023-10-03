#include "Storage.cpp"
#include "BPlusTree.cpp"
#include "Record.cpp"
#include "Storage.h"
#include "BPlusTree.h"
#include "Record.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <unordered_map>

using namespace std;

std::vector<std::string> splitStringByTab(const std::string &str)
{
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string item;

  while (std::getline(ss, item, '\t'))
  {
    std::size_t startPos = item.find_first_not_of("\t\n\r\f\v");
    // cout<<"testing"<<(item)<<"loc"<<startPos<<endl;
    if (startPos == 0)
      result.push_back(item);
  }

  return result;
}

int main()
{
  /*
    =============================================================
    Experiment 1:
    Store the data (which is about IMDb movives and described in Part 4) on the disk and report the following statistics:
    - The number of blocks;
    - The size of database;
    =============================================================
  */

  int BLOCKSIZE = int(400);

  // Create memory pools for the disk and the index, total 500MB
  // The split is determined empirically. We split so that we can have a contiguous disk address space for records
  std::cout << "creating the disk on the stack for records, index" << endl;
  Storage disk(100000000, BLOCKSIZE);  // 100MB
  Storage index(500000000, BLOCKSIZE); // 500MB

  // Creating the tree
  BPTree tree = BPTree();
  std::cout << "Max keys for a B+ tree node: " << tree.getMaxKey() << endl;

  // Reset the number of blocks accessed to zero
  disk.resetBlocksAccessed();
  index.resetBlocksAccessed();
  std::cout << "Number of record blocks accessed in search operation reset to: 0" << endl;
  std::cout << "Number of index blocks accessed in search operation reset to: 0" << endl;

  // Open test data
  std::cout << "Reading in data ... " << endl;
  std::ifstream file("../dataset/games.txt"); // actual data

  int recordCnt = 0;
  // Insert data into database and populate list of addresses
  if (file.is_open())
  {
    cout << "file is open" << endl;
    std::string line;
    int recordNum = 0;
    std::getline(file, line);
    while (std::getline(file, line))
    {
      // temporary struct Record
      // Record temp;
      stringstream linestream(line);
      std::vector<std::string> output = splitStringByTab(line);
      
      std::cout << std::endl;
      //cout << "the size is: " << output.size() << endl;
      if (output.size() < 9)
      {
        continue;
      }
      Record newRec(output[0], stoll(output[1]), stoi(output[2]), stod(output[3]), stod(output[4]), stod(output[5]),
                    stoi(output[6]), stoi(output[7]), stoi(output[8]), 0);
      //cout<<"The size is: "<<sizeof(newRec)<<endl;
      disk.insertToDisk(&newRec, sizeof(newRec));
      recordCnt++;
    }
    file.close();

    for (int i = 0; i < 60; ++i) {
      std::cout << '-';
    }
    std::cout << std::endl;

    cout<<"The configuration of Memory Pool:"<<endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(30) << std::left << "Parameter" 
              << std::setw(30) << std::left << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Print the table content
    std::cout << std::setw(30) << std::left << "Block size:" 
              << std::setw(30) << std::left << "400B" << std::endl;

    std::cout << std::setw(30) << std::left << "Total Number of Blocks:" 
              << std::setw(30) << std::left << "100" << std::endl;

    std::cout << std::setw(30) << std::left << "Used Number of Blocks:" 
              << std::setw(30) << std::left << "50" << std::endl;

    std::cout << std::setw(30) << std::left << "Disk Capacity:" 
              << std::setw(30) << std::left << "500MB" << std::endl;

    std::cout << std::setw(30) << std::left << "Used Disk Capacity:" 
              << std::setw(30) << std::left << "100MB" << std::endl;

    // Print the table footer
    std::cout << std::string(60, '-') << std::endl;

    
    cout<<"1. The number of records:\t"<<recordCnt<<endl;
    cout<<"2. The size of a record: \t"<<disk.getUsedMemorySize()/recordCnt<<endl;
    cout<<"3. The number of records stored in a block (average): \t"<<recordCnt/disk.getCurrentBlockCount()<<endl;
    cout<<"4. The number of blocks for storing the data: \t"<<disk.getCurrentBlockCount()<<endl;

  }
  else
  {
    cout << "File unfound." << endl;
  }
}