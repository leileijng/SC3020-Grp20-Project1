#include "Storage.h"
#include "BPlusTree.h"
#include "Record.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// Utility function to split a string by tab
std::vector<std::string> splitStringByTab(const std::string &str);
std::map<long long, Address *> recIndex2address;

// Exercise 1: Store data on disk and report statistics
void exercise1(std::vector<std::pair<Address *, long long> > &, Storage &disk);

// Exercise 2: Build B+ tree and report statistics
void exercise2(const std::vector<std::pair<Address *, long long> > &, BPTree &tree, Storage &disk);

// Exercise 3: Exact match search
void exercise3(const std::vector<std::pair<Address *, long long> > &, long long , BPTree &, Storage &);

// Exercise 4: Range search
void exercise4(const std::vector<std::pair<Address *, long long> > &, long long , long long , BPTree &, Storage &);

// Exercise 4: Range search
void exercise5(std::vector<std::pair<Address *, long long> > &, long long , BPTree &, Storage &);
 
// Main function
int main()
{
    int BLOCKSIZE = 400;
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

    std::vector<std::pair<Address *, long long> > addressIdVector;
    exercise1(addressIdVector, disk);

    BPTree tree(3);
    exercise2(addressIdVector, tree, disk);

    long long target = 50000000000000000LL;
    exercise3(addressIdVector, target, tree, disk);

    long long lowerBound = 60000000000000000LL;
    long long upperBound = 100000000000000000LL;
    exercise4(addressIdVector, lowerBound, upperBound, tree, disk);

    // exercise 5 - no yet done
    long long deleteUpperBound = 30000000000000000LL;
    exercise5(addressIdVector, deleteUpperBound, tree, disk);
}

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

void exercise1(std::vector<std::pair<Address *, long long> > &addressIdVector, Storage &disk)
{
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "                          Exercise 1                          " << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    //cout << "Reading in data ... " << endl;
    ifstream file("../dataset/games_final.txt"); // actual data

    if (file.is_open())
    {
        //cout << "file is open" << endl;
        string line;
        getline(file, line);
        while (getline(file, line))
        {
            stringstream linestream(line);
            vector<string> output = splitStringByTab(line);

            if (output.size() < 10)
            {
                continue;
            }

            Record newRec(output[0], stoll(output[1]), stoi(output[2]), stod(output[3]), stod(output[4]), stod(output[5]),
                          stoi(output[6]), stoi(output[7]), stoi(output[8]), 0);

            Address *addrOnDisk = new Address(disk.insertToDisk(&newRec, sizeof(newRec)));
            pair<Address *, long long> myPair(addrOnDisk, stoll(output[9]));
            addressIdVector.push_back(myPair);
        }
        file.close();

        int recordCnt = addressIdVector.size();

        //cout << string(60, '-') << endl;
        //cout << "The configuration of Memory Pool:" << endl;
        //cout << string(60, '-') << endl;
        cout << setw(30) << left << "Property" << setw(30) << left << "Value" << endl;
        cout << string(60, '-') << endl;

        cout << setw(30) << left << "Block size:" << setw(30) << left << "400B" << endl;
        cout << setw(30) << left << "Total Number of Blocks:" << setw(30) << left << disk.getTotalBlockCount() << endl;
        cout << setw(30) << left << "Used Number of Blocks:" << setw(30) << left << disk.getCurrentBlockCount() << endl;
        cout << setw(30) << left << "Disk Capacity:" << setw(30) << left << disk.getTotalMemorySize() << endl;
        cout << setw(30) << left << "Used Disk Capacity:" << setw(30) << left << disk.getUsedMemorySize() << endl;

        cout << string(60, '-') << endl;
        cout << setw(30) << left << "**Number of Records:**" << setw(30) << left << recordCnt << endl;
        cout << setw(30) << left << "**Size of a Record:**" << setw(30) << left << disk.getUsedMemorySize() / recordCnt << endl;
        cout << setw(30) << left << "**Records per Block:**" << setw(30) << left << (recordCnt - (disk.getUsedBlockCapacity() / sizeof(Record))) / (disk.getCurrentBlockCount() - 1) << endl;
        cout << setw(30) << left << "**Blocks for Storing Data:**" << setw(30) << left << disk.getCurrentBlockCount() << endl;

        cout << string(60, '-') << endl;
        cout << endl;
    }
    else
    {
        cout << "File unfound." << endl;
    }
}

void exercise2(const std::vector<std::pair<Address *, long long> > &addressIdVector, BPTree &tree, Storage &disk)
{
    std::cout << std::string(60, '*') << std::endl;
    std::cout << "                          Exercise 2                          " << std::endl;
    std::cout << std::string(60, '*') << std::endl;
    for (const auto &pair : addressIdVector)
    {
        Address* address = pair.first;
        long long recIndex = pair.second;

        if(recIndex2address.find(recIndex) == recIndex2address.end()){
            recIndex2address[recIndex] = address;
            tree.insert(recIndex, address);
        }
        
        Record *record = static_cast<Record *>(disk.loadFromDisk(*recIndex2address[recIndex], sizeof(Record)));
        record->setCount(record->getCount() + 1);
        delete record;
    }

    //tree.display(tree.getRoot(), 1);

    std::cout << std::setw(30) << std::left << "Property"
              << std::setw(30) << std::left << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Print the table content
    std::cout << std::setw(30) << std::left << "Parameter n of B+ Tree:"
              << std::setw(30) << std::left << tree.getMaxKey() << std::endl;

    std::cout << std::setw(30) << std::left << "number of nodes:"
              << std::setw(30) << std::left << tree.getNumNodes() << std::endl;

    std::cout << std::setw(30) << std::left << "number of levels:"
              << std::setw(30) << std::left << tree.getNumLevels() << std::endl;

    std::cout << std::setw(30) << std::left << "Content of Root:"
              << std::setw(30) << std::left; 
              tree.displayNode(tree.getRoot());

    // Print the table footer
    std::cout << std::string(60, '-') << std::endl;
    std::cout << endl;
}

// Exercise 3: Exact match search
void exercise3(const std::vector<std::pair<Address *, long long> > &addressIdVector, long long target, BPTree &tree, Storage &disk)
{
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int indexNodeCount = 0;
    int indexNodeLeafCount = 0;
    int dataBlockCount = 0;

    std::vector<Address *> result = tree.searchExact(target, indexNodeCount, indexNodeLeafCount);

    // Display the statistics
    std::cout << "Number of index nodes (non-leaf) accessed: " << indexNodeCount << std::endl;
    std::cout << "Number of index nodes (leaf) accessed: " << indexNodeLeafCount << std::endl;
    // std::cout << "Number of data blocks accessed: " << dataBlockCount << std::endl;
    if (result.empty())
        return;
    float sum_FG3_PCT_home = 0;
    int countOfRecords = 0;
    for (Address *address : result)
    {
        if (address == nullptr)
        {
            std::cout << "Address is null. No data to display." << std::endl;
            continue;
        }

        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));
        if (record != nullptr)
        {
            sum_FG3_PCT_home += record->getFg3PctHome() * record->getCount();
            countOfRecords += record->getCount();
            // record->display();  // Assuming you have a display() method in your Record class
            delete record;
        }
    }

    std::cout << "The total number of records: " << countOfRecords << std::endl;
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << sum_FG3_PCT_home / countOfRecords << std::endl;

    float targetFgPctHome = 0.5;
    // brute force search
    cout<<"------------Brute Force!!"<<endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    std::vector<Record> matchingRecords;
    float bf_sum_FG3_PCT_home = 0;
    auto start = std::chrono::high_resolution_clock::now();  // Start time
    for (const auto &pair : addressIdVector)
    {
        Address *address = pair.first;

        if (address == nullptr)
        {
            std::cout << "Address is null. Skipping this record." << std::endl;
            continue;
        }
        // Load the record from disk
        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));

        if (record != nullptr)
        {
            if (record->getFgPctHome() == targetFgPctHome)
            { // Assuming you have a getFgPctHome() method in your Record class
                bf_sum_FG3_PCT_home += record->getFg3PctHome();
                matchingRecords.push_back(*record);
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }


    auto stop = std::chrono::high_resolution_clock::now();  // Stop time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    // Display the statistics
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << bf_sum_FG3_PCT_home / matchingRecords.size() << std::endl;
    std::cout << "The total number of records between xxx and xxx: " << matchingRecords.size() << endl;
}

// Exercise 4: Range search
void exercise4(const std::vector<std::pair<Address *, long long> > &addressIdVector, long long lowerBound, long long upperBound, BPTree &tree, Storage &disk)
{
    cout << "-----------------------exercise 4-------------------------" << endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int indexNodeCount = 0;
    int indexNodeLeafCount = 0;
    int dataBlockCount = 0;
    double avgFg3PctHome = 0.0;

    std::vector<Address *> result = tree.searchRange(lowerBound, upperBound, indexNodeCount, indexNodeLeafCount);

    // Display the statistics
    std::cout << "Number of index nodes (non-leaf) accessed: " << indexNodeCount << std::endl;
    std::cout << "Number of index nodes (leaf) accessed: " << indexNodeLeafCount << std::endl;
    // std::cout << "Number of data blocks accessed: " << dataBlockCount << std::endl;
    if (result.empty())
        return;
    float sum_FG3_PCT_home = 0;
    int countOfRecords = 0;
    for (Address *address : result)
    {
        if (address == nullptr)
        {
            std::cout << "Address is null. No data to display." << std::endl;
            continue;
        }

        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));
        if (record != nullptr)
        {
            sum_FG3_PCT_home += record->getFg3PctHome() * record->getCount();
            countOfRecords += record->getCount();
            // record->display();  // Assuming you have a display() method in your Record class
            delete record;
        }
    }

    std::cout << "The total number of records: " << countOfRecords << std::endl;
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << sum_FG3_PCT_home / countOfRecords << std::endl;


    // brute force search
    cout<<"------------Brute Force!!"<<endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    std::vector<Record> matchingRecords;
    float bf_sum_FG3_PCT_home = 0;
    auto start = std::chrono::high_resolution_clock::now();  // Start time
    for (const auto &pair : addressIdVector)
    {
        Address *address = pair.first;

        if (address == nullptr)
        {
            std::cout << "Address is null. Skipping this record." << std::endl;
            continue;
        }
        // Load the record from disk
        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));

        if (record != nullptr)
        {
            if (record->getFgPctHome() >= 0.6 && record->getFg3PctHome() <= 1)
            { // Assuming you have a getFgPctHome() method in your Record class
                bf_sum_FG3_PCT_home += record->getFg3PctHome();
                matchingRecords.push_back(*record);
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }


    auto stop = std::chrono::high_resolution_clock::now();  // Stop time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    // Display the statistics
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << bf_sum_FG3_PCT_home / matchingRecords.size() << std::endl;
    std::cout << "The total number of records between "<<lowerBound <<" and " << upperBound << "is: " << matchingRecords.size() << endl;

}

// Exercise 5: Range remove
void exercise5(std::vector<std::pair<Address *, long long> > &addressIdVector, long long deleteUpperBound, BPTree &tree, Storage &disk){
   
    std::cout << std::setw(30) << std::left << "Before deletion: Used Disk Capacity:"
                      << std::setw(30) << std::left << disk.getUsedMemorySize() << std::endl;   
    std::cout << "The number of nodes before deletion: " << tree.countNodes(tree.getRoot()) << std::endl;
    std::cout << "The number of levels before deletion: " << tree.getNumLevels() << std::endl;

   std::vector<Address*> deletedRecords = tree.removeKeysBelow(deleteUpperBound);
   
    if (deletedRecords.empty())
        return;
    int countOfRecords = 0;
    for (Address *address : deletedRecords)
    {
        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));
        if (record != nullptr)
        {
            countOfRecords += record->getCount();
            // record->display();  // Assuming you have a display() method in your Record class
            delete record;
        }
    }

    std::cout << "The total number of records to delete: " << countOfRecords << std::endl;
    std::cout << "The number of nodes after deletion: " << tree.countNodes(tree.getRoot()) << std::endl;
    std::cout << "The number of levels after deletion: " << tree.getNumLevels() << std::endl;
   


     // brute force search
    cout<<"------------Brute Force!!"<<endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int recordCnt = 0;
    float bf_sum_FG3_PCT_home = 0;
    auto start = std::chrono::high_resolution_clock::now();  // Start time
    for (const auto &pair : addressIdVector)
    {
        Address *address = pair.first;

        if (address == nullptr)
        {
            std::cout << "Address is null. Skipping this record." << std::endl;
            continue;
        }
        // Load the record from disk
        Record *record = static_cast<Record *>(disk.loadFromDisk(*address, sizeof(Record)));

        if (record != nullptr)
        {
            if (record->getFg3PctHome() <= 0.35)
            { 
                recordCnt += 1;
                disk.deallocate(*address, sizeof(Record));
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }


    auto stop = std::chrono::high_resolution_clock::now();  // Stop time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    // Display the statistics
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << std::setw(30) << std::left << "Used Disk Capacity:"
                      << std::setw(30) << std::left << disk.getUsedMemorySize() << std::endl;       
}