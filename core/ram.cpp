#include "ram.hpp"

void RAM::write(uint32_t address, uint8_t data) {
    RAM::Memory[address] = data;
}

uint8_t RAM::read(uint32_t address) {
    return RAM::Memory[address];
}