#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <sys/types.h>
#include "Record.h"

class Memory {
private:
    std::size_t memSize;
    std::size_t memSizeUsed;
    std::size_t blockSize;
    std::size_t blockSizeUsed;

    int blockCount;


public:
    Memory(std::size_t memSize, std::size_t blockSize);
    
    bool allocateBlock();

    Address allocateData(Record record);

    bool deallocate(Address address, Record record_to_delete);

    Address save();

    std::size_t getBlockSize();

    std::size_t getBlockSizeUsed();

    std::size_t getMemSize();

    std::size_t getMemSizeUsed();

    int getBlockCount();

};

#endif