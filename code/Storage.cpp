#include "Storage.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

Storage::Storage(std::size_t memorySize, std::size_t blockSize)
    : totalMemorySize(memorySize), blockCapacity(blockSize), totalBlockCount(memorySize / blockSize),
      usedMemorySize(0), usedBlockCapacity(0), currentBlockCount(0), blocksAccessedCount(0)
{
    memoryPool = operator new(memorySize);
    std::memset(memoryPool, '\0', memorySize); // Initialize memory pool to null.
    currentBlock = nullptr;
}

bool Storage::allocateNewBlock()
{
    if (usedMemorySize + blockCapacity > totalMemorySize) return false;

    currentBlock = static_cast<char *>(memoryPool) + currentBlockCount * blockCapacity;
    usedBlockCapacity = 0;
    currentBlockCount++;
    return true;
}

Address Storage::allocateData(std::size_t requiredSize)
{
    if (requiredSize > blockCapacity)
    {
        std::cout << "Error: Required size is larger than block capacity (" << requiredSize << " vs " << blockCapacity << "). Increase block size to store data.\n";
        throw std::invalid_argument("Requested size too large!");
    }

    if (currentBlockCount == 0 || requiredSize > blockCapacity - usedBlockCapacity)
    {
        if (!allocateNewBlock())
        {
            throw std::logic_error("Failed to allocate new block - no more space!");
        }
    }

    short int offset = usedBlockCapacity;
    usedMemorySize += requiredSize;
    usedBlockCapacity += requiredSize;

    return Address(currentBlock, offset);
}

// Deallocate data from a given address and size.
bool Storage::deallocate(Address address, std::size_t sizeToDelete)
{
    try
    {
        // Calculate the address to delete.
        void *addressToDelete = static_cast<char *>(address.getBlockAddress()) + address.getOffset();
        
        // Set the memory area to null.
        std::memset(addressToDelete, '\0', sizeToDelete);

        // Check if the block is empty.
        unsigned char testBlock[blockCapacity];
        std::memset(testBlock, '\0', blockCapacity);

        // If the block is empty, update the memory size and block count.
        if (std::memcmp(testBlock, address.getBlockAddress(), blockCapacity) == 0)
        {
            usedMemorySize -= blockCapacity;
            currentBlockCount--;
        }

        // Update the actual size used.
        usedMemorySize -= sizeToDelete;
        return true;
    }
    catch (...)
    {
        std::cout << "Error: Could not remove record/block at given address and offset.\n";
        return false;
    }
}

// Load data from disk to main memory.
void *Storage::loadFromDisk(Address address, std::size_t size)
{
    void *mainMemoryAddress = operator new(size);
    std::memcpy(mainMemoryAddress, static_cast<char *>(address.getBlockAddress()) + address.getOffset(), size);
    return mainMemoryAddress;
}

// Save data to disk.
Address Storage::insertToDisk(void *itemAddress, std::size_t size)
{
    Address diskAddress = allocateData(size);
    std::memcpy(static_cast<char *>(diskAddress.getBlockAddress()) + diskAddress.getOffset(), itemAddress, size);
    return diskAddress;
}

// Update data on disk.
Address Storage::updateToDisk(void *itemAddress, std::size_t size, Address diskAddress)
{
    std::memcpy(static_cast<char *>(diskAddress.getBlockAddress()) + diskAddress.getOffset(), itemAddress, size);
    return diskAddress;
}

// Getter methods
std::size_t Storage::getBlockCapacity() const { return blockCapacity; }
std::size_t Storage::getUsedBlockCapacity() const { return usedBlockCapacity; }
std::size_t Storage::getTotalMemorySize() const { return totalMemorySize; }
std::size_t Storage::getUsedMemorySize() const { return usedMemorySize; }
int Storage::getCurrentBlockCount() const { return currentBlockCount; }

void Storage::resetBlocksAccessed()
  {
    int tempBlocksAccessed = 0;
  }
