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
void exercise3(const std::vector<std::pair<Address *, long long> > &, long long, BPTree &, Storage &);

// Exercise 4: Range search
void exercise4(const std::vector<std::pair<Address *, long long> > &, long long, long long, BPTree &, Storage &);

// Exercise 4: Range search
void exercise5(std::vector<std::pair<Address *, long long> > &, long long, BPTree &, Storage &);

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

    BPTree tree(20);
    exercise2(addressIdVector, tree, disk);

    long long target = 50000000000000000LL;
    exercise3(addressIdVector, target, tree, disk);

    // long long lowerBound = 60000000000000000LL;
    // long long upperBound = 100000000000000000LL;
    // exercise4(addressIdVector, lowerBound, upperBound, tree, disk);

    // // exercise 5 - no yet done
    // long long deleteUpperBound = 30000000000000000LL;
    // exercise5(addressIdVector, deleteUpperBound, tree, disk);
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

    // cout << "Reading in data ... " << endl;
    ifstream file("../dataset/games_final.txt"); // actual data

    if (file.is_open())
    {
        // cout << "file is open" << endl;
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

        // cout << string(60, '-') << endl;
        // cout << "The configuration of Memory Pool:" << endl;
        // cout << string(60, '-') << endl;
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
    // long long maxId = 0;
    for (const auto &pair : addressIdVector)
    {
        Address *address = pair.first;
        long long recIndex = pair.second;

        if (recIndex2address.find(recIndex) == recIndex2address.end())
        {
            recIndex2address[recIndex] = address;
            tree.insert(recIndex, address);
            // maxId = max(maxId, recIndex);
        }

        Record *record = static_cast<Record *>(disk.loadFromDisk(*recIndex2address[recIndex], sizeof(Record)));
        record->setCount(record->getCount() + 1);
        disk.updateToDisk(record, sizeof(Record), *recIndex2address[recIndex]);
        delete record;
    }
    // printf("max:%lld\n", maxId);

    // tree.display(tree.getRoot(), 1);

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

    // tree.travel(tree.getRoot());
}

// Exercise 3: Exact match search
void exercise3(const std::vector<std::pair<Address *, long long> > &addressIdVector, long long target, BPTree &tree, Storage &disk)
{
    std::cout << std::string(60, '*') << std::endl;
    std::cout << "                          Exercise 3                          " << std::endl;
    std::cout << std::string(60, '*') << std::endl;

    // Initialize variables for B+ Tree search
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int indexNodeCount = 0;
    int indexNodeLeafCount = 0;
    auto bptree_start = std::chrono::high_resolution_clock::now();
    std::vector<Address *> result = tree.searchExact(target, indexNodeCount, indexNodeLeafCount);
    auto bptree_stop = std::chrono::high_resolution_clock::now();
    auto bptree_duration = std::chrono::duration_cast<std::chrono::microseconds>(bptree_stop - bptree_start);

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

    int blkAccessForBTreeSearch = disk.getUniqueBlocksAccessed();
    // Statistics
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(30) << std::left << "Metric"
              << std::setw(30) << std::left << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    std::cout << std::setw(30) << std::left << "Total Number of Records"
              << std::setw(30) << std::left << countOfRecords << std::endl;

    std::cout << std::setw(30) << std::left << "Index Node Accessed"
              << std::setw(30) << std::left << indexNodeCount + indexNodeLeafCount << std::endl;

    std::cout << std::setw(30) << std::left << "Data Blocks Accessed"
              << std::setw(30) << std::left << blkAccessForBTreeSearch << std::endl;

    std::cout << std::setw(30) << std::left << "Average of FG3_PCT_home"
              << std::setw(30) << std::left << sum_FG3_PCT_home / countOfRecords << std::endl;

    std::cout << std::string(60, '=') << std::endl;

    // Initialize variables for Brute Force search
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    float bf_sum_FG3_PCT_home = 0;
    std::vector<Record> matchingRecords;
    auto bf_start = std::chrono::high_resolution_clock::now();
    // bf
    int cnt = 0;
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
            if (record->getFgPctHome() == 0.5)
            { // Assuming you have a getFgPctHome() method in your Record class
                bf_sum_FG3_PCT_home += record->getFg3PctHome() * record->getCount();
                matchingRecords.push_back(*record);
                cnt += record->getCount();
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }

    auto bf_stop = std::chrono::high_resolution_clock::now();
    auto bf_duration = std::chrono::duration_cast<std::chrono::microseconds>(bf_stop - bf_start);

    std::cout << "       Comparison Between B+Tree and Brute Force                 " << std::endl;

    std::cout << std::string(60, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "Metric"
              << std::setw(20) << std::left << "B+ Tree"
              << std::setw(20) << std::left << "Brute Force" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    std::cout << std::setw(20) << std::left << "Number of Records"
              << std::setw(20) << std::left << countOfRecords
              << std::setw(20) << std::left << cnt << std::endl;

    std::cout << std::setw(20) << std::left << "Blocks Accessed"
              << std::setw(20) << std::left << blkAccessForBTreeSearch
              << std::setw(20) << std::left << disk.getUniqueBlocksAccessed() << std::endl;

    std::cout << std::setw(20) << std::left << "Access Time (ms)"
              << std::setw(20) << std::left << bptree_duration.count()
              << std::setw(20) << std::left << bf_duration.count() << std::endl;

    std::cout << std::string(60, '=') << std::endl;
    std::cout << endl;
}


// Exercise 4: Range search
void exercise4(const std::vector<std::pair<Address *, long long> > &addressIdVector, long long lowerBound, long long upperBound, BPTree &tree, Storage &disk)
{
    std::cout << std::string(60, '+') << std::endl;
    std::cout << "                          Exercise 4                          " << std::endl;
    std::cout << std::string(60, '+') << std::endl;
    
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int indexNodeCount = 0;
    int indexNodeLeafCount = 0;

    auto bptree_start = std::chrono::high_resolution_clock::now();
    std::vector<Address *> result = tree.searchRange(lowerBound, upperBound, indexNodeCount, indexNodeLeafCount);
    auto bptree_stop = std::chrono::high_resolution_clock::now();
    auto bptree_duration = std::chrono::duration_cast<std::chrono::microseconds>(bptree_stop - bptree_start);

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
            delete record;
        }
    }

    int blkAccessForBTreeSearch = disk.getUniqueBlocksAccessed();

    // Statistics
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(30) << std::left << "Metric"
              << std::setw(30) << std::left << "Value" << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    std::cout << std::setw(30) << std::left << "Total Number of Records"
              << std::setw(30) << std::left << countOfRecords << std::endl;

    std::cout << std::setw(30) << std::left << "Index Node Accessed"
              << std::setw(30) << std::left << indexNodeCount + indexNodeLeafCount << std::endl;

    std::cout << std::setw(30) << std::left << "Data Blocks Accessed"
              << std::setw(30) << std::left << blkAccessForBTreeSearch << std::endl;

    std::cout << std::setw(30) << std::left << "Average of FG3_PCT_home"
              << std::setw(30) << std::left << sum_FG3_PCT_home / countOfRecords << std::endl;

    std::cout << std::string(60, '=') << std::endl;


    // brute force search
    
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    std::vector<Record> matchingRecords;
    float bf_sum_FG3_PCT_home = 0;
    auto bf_start = std::chrono::high_resolution_clock::now(); // Start time
    int cnt = 0;
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
            if (record->getFgPctHome() >= 0.6 && record->getFgPctHome() <= 1)
            { // Assuming you have a getFgPctHome() method in your Record class
                bf_sum_FG3_PCT_home += record->getFg3PctHome();
                matchingRecords.push_back(*record);
                cnt += record->getCount();
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }

    auto bf_stop = std::chrono::high_resolution_clock::now();
    auto bf_duration = std::chrono::duration_cast<std::chrono::microseconds>(bf_stop - bf_start);

    
    std::cout << "       Comparison Between B+Tree and Brute Force                 " << std::endl;

    std::cout << std::string(60, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "Metric"
              << std::setw(20) << std::left << "B+ Tree"
              << std::setw(20) << std::left << "Brute Force" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    std::cout << std::setw(20) << std::left << "Number of Records"
              << std::setw(20) << std::left << countOfRecords
              << std::setw(20) << std::left << cnt << std::endl;

    std::cout << std::setw(20) << std::left << "Blocks Accessed"
              << std::setw(20) << std::left << blkAccessForBTreeSearch
              << std::setw(20) << std::left << disk.getUniqueBlocksAccessed() << std::endl;

    std::cout << std::setw(20) << std::left << "Access Time (ms)"
              << std::setw(20) << std::left << bptree_duration.count()
              << std::setw(20) << std::left << bf_duration.count() << std::endl;

    std::cout << std::string(60, '=') << std::endl;
    std::cout << endl;
    
}

// Exercise 5: Range remove
void exercise5(std::vector<std::pair<Address *, long long> > &addressIdVector, long long deleteUpperBound, BPTree &tree, Storage &disk)
{

    std::cout << std::setw(30) << std::left << "Before deletion: Used Disk Capacity:"
              << std::setw(30) << std::left << disk.getUsedMemorySize() << std::endl;
    std::cout << "The number of nodes before deletion: " << tree.countNodes(tree.getRoot()) << std::endl;
    std::cout << "The number of levels before deletion: " << tree.getNumLevels() << std::endl;

    std::vector<Address *> deletedRecords = tree.removeKeysBelow(deleteUpperBound);

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
    cout << "------------Brute Force!!" << endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    int recordCnt = 0;
    float bf_sum_FG3_PCT_home = 0;
    auto start = std::chrono::high_resolution_clock::now(); // Start time
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
            if (record->getFgPctHome() <= 0.35)
            {
                recordCnt += record->getCount();
                disk.deallocate(*address, sizeof(Record));
            }
            delete record; // Don't forget to delete the dynamically allocated memory
        }
    }

    auto stop = std::chrono::high_resolution_clock::now(); // Stop time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    std::cout << "Records to delete: " << recordCnt << std::endl;
    // Display the statistics
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << std::setw(30) << std::left << "Used Disk Capacity:"
              << std::setw(30) << std::left << disk.getUsedMemorySize() << std::endl;
}