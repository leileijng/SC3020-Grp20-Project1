#include <stdexcept>

struct Address
{
    void *blockAddress;
    short int offset;

    Address() : blockAddress(nullptr), offset(0) {}
    Address(void *blockAddress, short int offset) : blockAddress(blockAddress), offset(offset) {}

    bool isValid() const {
        return blockAddress != nullptr;
    }
};
