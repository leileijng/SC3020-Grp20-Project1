#include "Memory.h"
#include "Record.h"

Memory::Memory(std::size_t memSize, std::size_t blockSize){
    this->memSize = memSize;
    this->blockSize = blockSize;
    this->memSizeUsed = 0;
    this->blockSizeUsed = 0;
    this->blockCount = 0;
};

bool Memory::allocateBlock(){
    if (this->memSizeUsed + this->blockSize > this->memSize){
        return false;
    }
    this->memSizeUsed += this->blockSize;
    this->blockSizeUsed += this->blockSize;
    this->blockCount++;
    return true;
};

Address Memory::allocateData(Record record){
    if (record.size() > this->blockSize){
        //need to increase block size
    }

    else if(blockCount==0 || record.size() > this->blockSize - this->blockSizeUsed){
        //need to allocate new block
        if (!this->allocateBlock()){
            //failed to allocate new block
        }
    }

    int offset = blockSizeUsed;

    blockSizeUsed += record.size();

    //Update address
};

bool Memory::deallocate(Address address, Record record_to_delete){
    
};
    
Address Memory::save(Record record){
    Address dataAddress = allocateData(record);
    std::memcpy((char *)dataAddress.blockAddress + dataAddress.offset, &record, record.size());

    return dataAddress;
};

