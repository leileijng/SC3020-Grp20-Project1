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
  int BLOCKSIZE = int(400);

  // Create memory pools for the disk and the index, total 500MB
  // The split is determined empirically. We split so that we can have a contiguous disk address space for records
  std::cout << "creating the disk on the stack for records, index" << endl;
  Storage disk(100000000, BLOCKSIZE);  // 100MB
  Storage index(500000000, BLOCKSIZE); // 500MB

  // Reset the number of blocks accessed to zero
  disk.resetBlocksAccessed();
  index.resetBlocksAccessed();
  std::cout << "Number of record blocks accessed in search operation reset to: 0" << endl;
  std::cout << "Number of index blocks accessed in search operation reset to: 0" << endl;

  /*
    =============================================================
    Experiment 1:
    store the data  on the disk (as specified in Part 1) and report the following statistics:
    • the number of records;
    • the size of a record;
    • the number of records stored in a block;
    • the number of blocks for storing the data;
    =============================================================
  */

  std::vector<std::pair<Address *, long long> > addressIdVector;
  // Open test data
  std::cout << "Reading in data ... " << endl;
  std::ifstream file("../dataset/games_final.txt"); // actual data

  // Insert data into database and populate list of addresses
  if (file.is_open())
  {
    cout << "file is open" << endl;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
    {
      // temporary struct Record
      // Record temp;
      stringstream linestream(line);
      std::vector<std::string> output = splitStringByTab(line);

      // cout << "the size is: " << output.size() << endl;
      if (output.size() < 10)
      {
        continue;
      }
      // cout << output[9] <<endl;
      Record newRec(output[0], stoll(output[1]), stoi(output[2]), stod(output[3]), stod(output[4]), stod(output[5]),
                    stoi(output[6]), stoi(output[7]), stoi(output[8]));
      /// std::cout << "size of record is: " << newRec.size();
      // cout<<"The size is: "<<sizeof(newRec)<<endl;
      Address addrOnDisk = disk.insertToDisk(&newRec, sizeof(newRec));
      std::pair<Address *, long long> myPair(&addrOnDisk, stoll(output[9]));
      addressIdVector.push_back(myPair);
    }
    file.close();
    int recordCnt = addressIdVector.size();

    std::cout << std::string(60, '-') << std::endl;

    cout << "The configuration of Memory Pool:" << endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(30) << std::left << "Parameter"
              << std::setw(30) << std::left << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Print the table content
    std::cout << std::setw(30) << std::left << "Block size:" // self-define
              << std::setw(30) << std::left << BLOCKSIZE << std::endl;

    std::cout << std::setw(30) << std::left << "Total Number of Blocks:" //...
              << std::setw(30) << std::left << disk.getTotalBlockCount() << std::endl;

    std::cout << std::setw(30) << std::left << "Used Number of Blocks:" //...
              << std::setw(30) << std::left << disk.getCurrentBlockCount() << std::endl;

    std::cout << std::setw(30) << std::left << "Disk Capacity:" // self-define
              << std::setw(30) << std::left << disk.getTotalMemorySize() << std::endl;

    std::cout << std::setw(30) << std::left << "Used Disk Capacity:"
              << std::setw(30) << std::left << disk.getUsedMemorySize() << std::endl;

    // Print the table footer
    std::cout << std::string(60, '-') << std::endl;

    cout << "1. The number of records:\t" << recordCnt << endl;
    cout << "2. The size of a record: \t" << disk.getUsedMemorySize() / recordCnt << endl;
    cout << "3. The number of records stored in a block: \t" << recordCnt / disk.getCurrentBlockCount() << endl;
    cout << "4. The number of blocks for storing the data: \t" << disk.getCurrentBlockCount() << endl;
  }
  else
  {
    cout << "File unfound." << endl;
  }

  /*
   =============================================================
   Experiment 2:
   Build a B+ tree on the attribute "FG_PCT_home" by inserting the records sequentially and report the following statistics:
    • the parameter n of the B+ tree;
    • the number of nodes of the B+ tree;
    • the number of levels of the B+ tree;
    • the content of the root node (only the keys);
   =============================================================
   */
  

  BPTree tree(3);
  for (const auto &pair : addressIdVector)
  {
    tree.insert(pair.second, pair.first);
  }
  tree.display(tree.getRoot(), 1);

  cout << "Info about B+ Tree:" << endl;
  std::cout << std::string(60, '-') << std::endl;
  std::cout << std::setw(30) << std::left << "Parameter"
            << std::setw(30) << std::left << "Value" << std::endl;
  std::cout << std::string(60, '-') << std::endl;

  // Print the table content
  std::cout << std::setw(30) << std::left << "Parameter n of B+ Tree:"
            << std::setw(30) << std::left << tree.getMaxKey() << std::endl;

  std::cout << std::setw(30) << std::left << "number of nodes:"
            << std::setw(30) << std::left << tree.getNumNodes() << std::endl;

  std::cout << std::setw(30) << std::left << "number of levels:"
            << std::setw(30) << std::left << tree.getNumLevels() << std::endl;

  //std::cout << std::setw(30) << std::left << "Content of Root:"
   //         << std::setw(30) << std::left << tree.getRoot() << std::endl;

  // Print the table footer
  std::cout << std::string(60, '-') << std::endl;



  //exercise 3
  // format 

  // ask user for input (0.5) -> convert to the below format 

  // search experiment
  long long lowerBound = 60000000000000000LL;
  long long upperBound = 70000000000000000LL;
  vector<Address *> result = tree.searchRange(lowerBound, upperBound);
  if (result.empty())
  {
    std::cout << "No keys found in the range [" << lowerBound << ", " << upperBound << "]" << std::endl;
  }

  std::cout << "Keys found in the range [" << lowerBound << ", " << upperBound << "]:" << std::endl;
  for (Address *address : result)
  {
    if (address == nullptr)
    {
      std::cout << "Address is null. No data to display." << std::endl;
    }

    // Assuming you have a Storage object named storage
    Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));

    // Display the record
    //record->display();
    delete record;                                         // Don't forget to delete the dynamically allocated memory
    //std::cout << "-------------------------" << std::endl; // Separator between records
  }
}