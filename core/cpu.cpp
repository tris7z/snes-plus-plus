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
        if(!CPU::IMap.count(opcode)) {
            std::cout << "Unknown opcode!" << std::endl;
            this->dump_registers();
            return;
        }

        // does the order matter here?
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
    printf("A    X    Y    SP    DB    DP    PB    PS    PC\n");
    printf("%04x %04x %04x %04x  %04x  %04x  %04x  %04x  %04x\n",
        this->getRegisterA(),
        this->getRegisterX(),
        this->getRegisterY(),
        this->getRegisterSP(),
        this->getRegisterDB(),
        this->getRegisterDP(),
        this->getRegisterPB(),
        this->getRegisterPS(),
        this->getRegisterPC());
}

std::vector<uint8_t> CPU::HexToBytes(const std::string& hex) {
  std::vector<uint8_t> bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    uint8_t byte = (uint8_t) std::strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }

  return bytes;
}

void CPU::debugMode() {
    std::string input;

    while(true) {
        this->setRegisterPC(1); // we would increment before executing
        std::cout << "Enter an instruction byte (including any data): 0x";
        std::cin >> std::hex >> input;
        
        try {
            std::stoul(input, NULL, 16);
        } catch (const std::invalid_argument& ia) {
            std::cout << "Unrecognized input" << std::endl;
            continue;      
        }
        if(input.length() % 2)
            input = "0" + input; // even length string
        
        std::vector<uint8_t> dataBytes = this->HexToBytes(input);
        for(int i = 0; i < (int)dataBytes.size(); i++) {
            ram.write(i, dataBytes[i]);
        }

        if(CPU::IMap.count(dataBytes[0]))
        {
            (this->*CPU::IMap[dataBytes[0]])();
            this->dump_registers();
        }
        else
        {
            std::cout << "Unknown instruction" << std::endl;
        }

    }
}

int CPU::test(std::string text) {
    std::cout << text << std::endl;
    return 5;
}

//Instructions
//Flag stuff
int CPU::CLC() {
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
int CPU::LDX_i() { //TODO add negative flag check on this, INX, and LDA
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_INDEX_WIDTH) >> 4);
    uint8_t loadData;
    uint16_t registerX;
    int cycles;

    if(halfWidth)
    {
        loadData = this->ram.read(this->getRegisterPC());
        registerX = loadData;
        cycles = 2;
    }
    else {
        // load two bytes into register X
        loadData = this->ram.read(this->getRegisterPC());
        registerX = loadData;
        this->setRegisterPC(this->getRegisterPC()+1);
        loadData = this->ram.read(this->getRegisterPC());
        registerX += loadData << 8;
        cycles = 3;
    }
    this->setRegisterPC(this->getRegisterPC()+1);
    
    this->setRegisterX(registerX);
    if(registerX == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    return cycles;
}
int CPU::LDA_i() {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_ACCUM_WIDTH) >> 5);
    uint8_t loadData;
    uint16_t registerA; 
    int cycles;

    if(halfWidth)
    {
        loadData = this->ram.read(this->getRegisterPC());
        registerA = loadData;
        cycles = 2;
    }
    else {
        // load two bytes into register A, low half first
        loadData = this->ram.read(this->getRegisterPC());
        registerA = loadData;
        this->setRegisterPC(this->getRegisterPC()+1);
        loadData = this->ram.read(this->getRegisterPC());
        registerA += loadData << 8;
        cycles = 3;
    }
    this->setRegisterPC(this->getRegisterPC()+1);
    
    this->setRegisterA(registerA);
    if(registerA == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);

    return cycles;
}
int CPU::STA_AiX() { //TODO: Add halfwidth stuff here, if in 16 bit mode you add both bits to RAM
    //bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_ACCUM_WIDTH) >> 5);

    uint16_t data = this->ram.read(this->getRegisterPC());
    this->setRegisterPC(this->getRegisterPC()+1);
    data += this->ram.read(this->getRegisterPC()) << 8;
    this->setRegisterPC(this->getRegisterPC()+1);

    uint16_t addr = data + this->getRegisterX();

    this->ram.write(addr, (uint8_t)this->getRegisterA());
    return 6;
}

int CPU::STA_addr() {
    //bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_ACCUM_WIDTH) >> 5);

    uint16_t data = this->ram.read(this->getRegisterPC());
    this->setRegisterPC(this->getRegisterPC()+1);
    data += this->ram.read(this->getRegisterPC()) << 8;
    this->setRegisterPC(this->getRegisterPC()+1);

    this->ram.write(data, (uint8_t)this->getRegisterA());
    return 4;
}

int CPU::INX() {
    this->setRegisterX(this->getRegisterX()+1);
    if(this->getRegisterX() == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    return 2;
}

int CPU::CPX() {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_INDEX_WIDTH) >> 4);
    uint8_t loadData;
    int cycles;
    uint16_t data;
    
    if(halfWidth)
    {
        loadData = this->ram.read(this->getRegisterPC());
        data = loadData;
        cycles = 2;
    }
    else
    {
        loadData = this->ram.read(this->getRegisterPC());
        data = loadData;
        this->setRegisterPC(this->getRegisterPC()+1);
        loadData = this->ram.read(this->getRegisterPC());
        data += loadData << 8;
        cycles = 3;
    }
    this->setRegisterPC(this->getRegisterPC()+1);
    
    uint8_t result = this->getRegisterX() - data;
    if((result) & STATUS_NEGATIVE)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_NEGATIVE);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_NEGATIVE);
    if(result == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_ZERO);
    if(this->getRegisterX() >= result)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_CARRY);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_CARRY);

    return cycles;
}

int CPU::BNE() {
    bool equal = (bool)( (this->getRegisterPS() & CPU::STATUS_ZERO) >> 1);
    int8_t offset = this->ram.read(this->getRegisterPC());
    int cycles = 2;
    if(!equal) {
        this->setRegisterPC(this->getRegisterPC() + offset);
        cycles = 3;
    }
    this->setRegisterPC(this->getRegisterPC()+1);
    return cycles;
}

int CPU::TXS() {
    this->setRegisterSP(this->getRegisterX());
    return 2;
}