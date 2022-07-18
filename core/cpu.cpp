#include "cpu.hpp"

//Setters
void CPU::setRegisterA(std::uint16_t value) {
    this->A = value;
}
void CPU::setRegisterX(std::uint16_t value) {
    this->X = value;
}
void CPU::setRegisterY(std::uint16_t value) {
    this->Y = value;
}
void CPU::setRegisterSP(std::uint16_t value) {
    this->SP = value;
}
void CPU::setRegisterDB(std::uint16_t value) {
    this->DB = value;
}
void CPU::setRegisterDP(std::uint16_t value) {
    this->DP = value;
}
void CPU::setRegisterPB(std::uint16_t value) {
    this->PB = value;
}
void CPU::setRegisterPS(std::uint8_t value) {
    this->PS = value;
}
void CPU::setRegisterPC(std::uint16_t value) {
    this->PC = value;
}

//Getters
std::uint16_t CPU::getRegisterA() {
    return this->A;
}
std::uint16_t CPU::getRegisterX() {
    return this->X;
}
std::uint16_t CPU::getRegisterY() {
    return this->Y;
}
std::uint16_t CPU::getRegisterSP() {
    return this->SP;
}
std::uint16_t CPU::getRegisterDB() {
    return this->DB;
}
std::uint16_t CPU::getRegisterDP() {
    return this->DP;
}
std::uint16_t CPU::getRegisterPB() {
    return this->PB;
}
std::uint8_t CPU::getRegisterPS() {
    return this->PS;
}
std::uint16_t CPU::getRegisterPC() {
    return this->PC;
}

void CPU::loadROM(std::string rom_path) {
    std::ifstream rom;
    rom.open(rom_path, std::ios::binary);

    uint32_t location = 0x8000;
    char romByte;
    while (!rom.eof()) {
        if(rom.eof()) break;
        rom.read(&romByte, 1);
        this->ram.write(location, romByte);
        location++;
    }

    rom.close();    
    this->setRegisterPC(0x8000);
}

void CPU::readAndExecute() {  
    while(true) {
        uint16_t PC = this->getRegisterPC();
        uint8_t opcode = this->ram.read(PC);
        std::cout << "Opcode: " << std::hex << (unsigned int)opcode << std::endl;
        this->setRegisterPC(PC+1);
        (this->*CPU::IMap[opcode])();
    }
}

void CPU::helloWorld() {
    std::cout << "Hello SNES!" << std::endl;
}

void CPU::printHex(uint16_t val) {
    std::cout << std::hex << (unsigned int)val << std::endl;
}

void CPU::dump_registers() {
    
}

void CPU::debugMode() {
    //this->dump_registers();
    while(true) {
        // ask for hex code, run function, print registers
    }
}

int CPU::test(std::string text) {
    std::cout << text << std::endl;
    return 5;
}

//Instructions
//Flag stuff
int CPU::CLC() {
    std::cout << "Running CLC" << std::endl;
    this->setRegisterPS(this->getRegisterPS() & ~(1 << 0));
    return 2;
};
int CPU::XCE() {
    return 2;
}
int CPU::SEP() {
    this->setRegisterPS(this->getRegisterPS() | this->ram.read(this->getRegisterPC()));
    this->setRegisterPC(this->getRegisterPC()+1);
    return 3;
}
int CPU::REP() {
    this->setRegisterPS(this->getRegisterPS() & ~this->ram.read(this->getRegisterPC()));
    this->setRegisterPC(this->getRegisterPC()+1);
    return 3;
}

//Loads
// todo: check flags for 8 or 16 bit (5th bit) + set cycles 
int CPU::LDXi() {
    this->setRegisterX(this->ram.read(this->getRegisterPC()));
    this->setRegisterPC(this->getRegisterPC()+1);
    return 2;
}