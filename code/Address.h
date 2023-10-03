#ifndef ADDRESS_H
#define ADDRESS_H

#include <stdexcept>

class Address {
private:
    void *blockAddress;
    short int offset;

public:
    // Constructors
    Address() : blockAddress(nullptr), offset(0) {}
    Address(void *blockAddress, short int offset) : blockAddress(blockAddress), offset(offset) {}

    // Check if the address is valid
    bool isValid() const;

    // Getter and Setter for blockAddress
    void* getBlockAddress() const;
    void setBlockAddress(void *value);

    // Getter and Setter for offset
    short int getOffset() const;
    void setOffset(short int value);
};

#endif // ADDRESS_H
