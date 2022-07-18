#include "cpu.hpp"

//Setters
void CPU::setRegisterA(std::uint16_t value) {
    A = value;
}
void CPU::setRegisterX(std::uint16_t value) {
    X = value;
}
void CPU::setRegisterY(std::uint16_t value) {
    Y = value;
}
void CPU::setRegisterSP(std::uint16_t value) {
    SP = value;
}
void CPU::setRegisterDB(std::uint16_t value) {
    DB = value;
}
void CPU::setRegisterDP(std::uint16_t value) {
    DP = value;
}
void CPU::setRegisterPB(std::uint16_t value) {
    PB = value;
}
void CPU::setRegisterPS(std::uint16_t value) {
    PS = value;
}
void CPU::setRegisterPC(std::uint16_t value) {
    PC = value;
}

//Getters
std::uint16_t CPU::getRegisterA() {
    return A;
}
std::uint16_t CPU::getRegisterX() {
    return X;
}
std::uint16_t CPU::getRegisterY() {
    return Y;
}
std::uint16_t CPU::getRegisterSP() {
    return SP;
}
std::uint16_t CPU::getRegisterDB() {
    return DB;
}
std::uint16_t CPU::getRegisterDP() {
    return DP;
}
std::uint16_t CPU::getRegisterPB() {
    return PB;
}
std::uint16_t CPU::getRegisterPS() {
    return PS;
}
std::uint16_t CPU::getRegisterPC() {
    return PC;
}

void CPU::readAndExecute(std::string filename) {
    std::ifstream rom;
    rom.open(filename, std::ios::binary);
    
    char romByte;
    rom.read(&romByte, 1);
    while (!rom.eof()) {
        std::cout << std::hex << (unsigned int)romByte << std::endl;
        rom.read(&romByte, 1);
        return;
    }
}

void CPU::helloWorld() {
    std::cout << "Hello SNES!" << std::endl;
}