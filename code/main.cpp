#include "Storage.h"
#include "BPlusTree.h"
#include "Record.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Utility function to split a string by tab
std::vector<std::string> splitStringByTab(const std::string &str);

// Exercise 1: Store data on disk and report statistics
void exercise1(std::vector<std::pair<Address *, long long> > &, Storage &disk);

// Exercise 2: Build B+ tree and report statistics
void exercise2(const std::vector<std::pair<Address *, long long> > &, BPTree &tree);

// Exercise 3: Exact match search
void exercise3(const std::vector<std::pair<Address *, long long> > &, long long target, BPTree &tree, Storage &disk);

// Exercise 4: Range search
void exercise4(const std::vector<std::pair<Address *, long long> > &, long long lowerBound, long long upperBound, BPTree &tree, Storage &disk);

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
    exercise2(addressIdVector, tree);

    long long target = 50000000000000000LL;
    exercise3(addressIdVector, target, tree, disk);

    long long lowerBound = 60000000000000000LL;
    long long upperBound = 100000000000000000LL;
    exercise4(addressIdVector, lowerBound, upperBound, tree, disk);

    // exercise 5 - no yet done
    long long deleteUpperBound = 30000000000000000LL;
    // tree.removeKeysBelow(deleteUpperBound);
    tree.remove(28947071061911480);
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
            Address *addrOnDisk = new Address(disk.insertToDisk(&newRec, sizeof(newRec)));

            // Now store the pointer in the pair
            std::pair<Address *, long long> myPair(addrOnDisk, stoll(output[9]));

            // Add the pair to the vector
            addressIdVector.push_back(myPair);
        }
        file.close();

        std::cout << std::string(60, '-') << std::endl;

        /*
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
            cout << "4. The number of blocks for storing the data: \t" << disk.getCurrentBlockCount() << endl;*/
    }
    else
    {
        cout << "File unfound." << endl;
    }
    int recordCnt = addressIdVector.size();
}

void exercise2(const std::vector<std::pair<Address *, long long> > &addressIdVector, BPTree &tree)
{
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

    // std::cout << std::setw(30) << std::left << "Content of Root:"
    //          << std::setw(30) << std::left << tree.getRoot() << std::endl;

    // Print the table footer
    std::cout << std::string(60, '-') << std::endl;
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
            sum_FG3_PCT_home += record->getFg3PctHome();
            // record->display();  // Assuming you have a display() method in your Record class
            delete record;
        }
    }

    // std::cout << "The total number of records: " << result.size() << std::endl;
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << sum_FG3_PCT_home / result.size() << std::endl;

    float targetFgPctHome = 0.5;
    // brute force search
    cout<<"------------Brute Force!!"<<endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    std::vector<Record> matchingRecords;
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
            sum_FG3_PCT_home += record->getFg3PctHome();
            // record->display();  // Assuming you have a display() method in your Record class
            delete record;
        }
    }

    // std::cout << "The total number of records: " << result.size() << std::endl;
    std::cout << "The number of data blocks the process accesses: " << disk.getUniqueBlocksAccessed() << std::endl;
    std::cout << "The number of data blocks the process accesses (non-unique): " << disk.getBlocksAccessedCount() << std::endl;
    std::cout << "Average of FG3_PCT_home: " << sum_FG3_PCT_home / result.size() << std::endl;


    // brute force search
    cout<<"------------Brute Force!!"<<endl;
    disk.resetUniqueBlocksAccessed();
    disk.resetBlocksAccessed();
    std::vector<Record> matchingRecords;
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

    std::cout << "The total number of records between "<<lowerBound <<" and " << upperBound << "is: " << matchingRecords.size() << endl;

}
