#include "Address.h"

// Check if the address is valid
bool Address::isValid() const {
    return blockAddress != nullptr;
}

// Getter for blockAddress
void* Address::getBlockAddress() const {
    return blockAddress;
}

// Setter for blockAddress
void Address::setBlockAddress(void *value) {
    blockAddress = value;
}

// Getter for offset
short int Address::getOffset() const {
    return offset;
}

// Setter for offset
void Address::setOffset(short int value) {
    offset = value;
}
