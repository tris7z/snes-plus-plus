#include "ram.hpp"

void RAM::write(uint32_t address, uint8_t data) {
    this->Memory[address] = data;
}

uint8_t RAM::read(uint32_t address) {
    return this->Memory[address];
}

void RAM::memoryDump() {
    std::ofstream output;
    output.open("memdump.hex", std::ios::out);
    if(!output)
        return;
    
    output << "0x000000: ";
    for(uint32_t i = 1; i <= 0xFFFFFF; i++)
    {
        output << std::hex << std::setfill('0') << std::setw(2) << (unsigned long)this->read(i-1) << " ";
        if(i % 16 == 0) {
            output << std::endl;
            output << "0x" << std::hex << std::setfill('0') << std::setw(6) << i << ": ";
        }
    }
    std::cout << "RAM dumped to memdump.hex" << std::endl;
    output.close();
}