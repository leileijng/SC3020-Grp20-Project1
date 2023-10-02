#ifndef STORAGE_H
#define STORAGE_H

#include <cstddef>
#include "Address.h"

class Storage
{
private:
    std::size_t totalMemorySize;    // Total size of the memory pool.
    std::size_t usedMemorySize;     // Memory used across all blocks.
    std::size_t blockCapacity;      // Size of each block.
    std::size_t usedBlockCapacity;  // Memory used in the current block.
    std::size_t totalBlockCount;    // Total number of blocks.

    int currentBlockCount;          // Number of currently allocated blocks.
    int blocksAccessedCount;        // Number of blocks accessed.

    void *memoryPool;               // Pointer to the memory pool.
    void *currentBlock;             // Pointer to the current block being accessed.

    void initializeMemoryPool();    // Initialize the memory pool.

public:
    Storage(std::size_t memorySize, std::size_t blockSize);

    bool allocateNewBlock();        // Allocate a new block in the memory pool.
    Address allocateData(std::size_t requiredSize); // Allocate data in the current block.
    bool deallocate(Address address, std::size_t sizeToDelete); // Deallocate data.

    // Getter methods
    std::size_t getBlockCapacity() const;
    std::size_t getUsedBlockCapacity() const;
    std::size_t getTotalMemorySize() const;
    std::size_t getUsedMemorySize() const;
    int getCurrentBlockCount() const;

    // Disk operations
    void *loadFromDisk(Address address, std::size_t size); // Load data from disk to main memory.
    Address insertToDisk(void *itemAddress, std::size_t size); // Save data to disk.
    Address updateToDisk(void *itemAddress, std::size_t size, Address diskAddress); // Update data on disk.
};

#endif
