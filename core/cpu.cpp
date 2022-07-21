#include "cpu.hpp"

CPU::CPU(RAM* ram) {
    this->setRAM(ram);
    this->zeroRegisters();
    readAndExecute();
}

CPU::CPU(RAM* ram, std::string option) {
    this->setRAM(ram);
    this->zeroRegisters();
    if(option == "debug")
    {
        this->debugMode();
    }
    if(option == "test")
    {
        this->test();
        this->getRAM()->memoryDump();
    }
}

//Setters
void CPU::setRAM(RAM* ram) {
    this->ram = ram;
}
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
void CPU::zeroRegisters() {
    this->setRegisterA(0);
    this->setRegisterX(0);
    this->setRegisterY(0);
    this->setRegisterSP(0);
    this->setRegisterDB(0);
    this->setRegisterDP(0);
    this->setRegisterPB(0);
    this->setRegisterPS(0);
    this->setRegisterPC(0);
}

//Getters
RAM* CPU::getRAM() {
    return this->ram;
}
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

uint8_t CPU::getProgramByte() {
    uint8_t program_byte = this->getRAM()->read(this->getRegisterPB() + this->getRegisterPC());
    this->setRegisterPC( this->getRegisterPC()+1 );
    return program_byte;
}

void CPU::readAndExecute() {
    this->setRegisterPB(0x80);
    this->setRegisterPC(0x8000);
    while(true)
    {
        uint8_t opcode = this->getProgramByte();
        std::cout << "Opcode: " << std::hex << (unsigned int)opcode << std::endl;
        
        if(!CPU::IMap.count(opcode)) {
            std::cout << "Unknown opcode!" << std::endl;
            this->dump_registers();
            return;
        }
        (this->*CPU::IMap[opcode])(opcode);
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
        if(input == "memdump") {
            this->getRAM()->memoryDump();
            continue;
        }

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
            this->getRAM()->write(i, dataBytes[i]);
        }

        if(CPU::IMap.count(dataBytes[0]))
        {
            (this->*CPU::IMap[dataBytes[0]])(dataBytes[0]);
            this->dump_registers();
        }
        else
        {
            std::cout << "Unknown instruction" << std::endl;
        }

    }
}

void CPU::test() {
    uint8_t data[5] = {0x69, 0x69, 0x42, 0x04, 0x20};
    for(int i = 0; i < 5; i++) {
        this->getRAM()->write(i, data[i]);
    }
}

//Instructions
//Flag stuff
int CPU::CLC(uint8_t opcode) {
    this->setRegisterPS(this->getRegisterPS() & ~(1 << 0));
    return 2;
};
int CPU::XCE(uint8_t opcode) {
    return 2;
}
int CPU::SEP(uint8_t opcode) {
    this->setRegisterPS(this->getRegisterPS() | this->getProgramByte());
    return 3;
}
int CPU::REP(uint8_t opcode) {
    this->setRegisterPS(this->getRegisterPS() & ~this->getProgramByte());
    return 3;
}

//Loads
int CPU::LDX(uint8_t opcode) {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_INDEX_WIDTH) >> 4);
    uint8_t loadData;
    uint16_t registerX;
    int cycles;
    switch(opcode) {
        case 0xA2:
        {
            // LDY #const  Immediate (2/3 bytes)
            cycles = 2;
            if(halfWidth)
            {
                loadData = this->getProgramByte();
                registerX = loadData;
                cycles = 2;
            }
            else {
                // load two bytes into register X
                loadData = this->getProgramByte();
                registerX = loadData;
                loadData = this->getProgramByte();
                registerX += loadData << 8;
                cycles = 3;
            }
            break;
        }
        case 0xAE:
            // LDY addr  Absolute (3 bytes)
            cycles = 4;

            break;
        case 0xA6:
            // LDY dp  Direct Page (2 bytes)
            cycles = 3;

            break;
        case 0xBE:
            // LDY addr, X  Absolute Indexed, X (3 bytes)
            cycles = 4;

            break;
        case 0xB6:
            // LDY dp, X  Direct Page Indexed, X (2 bytes)
            cycles = 4;

            break;
        default:
            break;
    }
    
    this->setRegisterX(registerX);
    if(registerX == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    if((registerX) & STATUS_NEGATIVE)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_NEGATIVE);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_NEGATIVE);
    return cycles;
}
int CPU::LDY(uint8_t opcode) {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_INDEX_WIDTH) >> 4);
    uint16_t registerY;
    uint8_t loadData;
    int cycles;
    switch(opcode) {
        case 0xA0:
            // LDY #const  Immediate (2/3 bytes)
            cycles = 2;
            if(halfWidth)
            {
                loadData = this->getProgramByte();
                registerY = loadData;
                cycles = 2;
            }
            else {
                // load two bytes into register X
                loadData = this->getProgramByte();
                registerY = loadData;
                loadData = this->getProgramByte();
                registerY += loadData << 8;
                cycles = 3;
            }
            break;
        case 0xAC:
            // LDY addr  Absolute (3 bytes)
            cycles = 4;

            break;
        case 0xA4:
            // LDY dp  Direct Page (2 bytes)
            cycles = 3;

            break;
        case 0xBC:
            // LDY addr, X  Absolute Indexed, X (3 bytes)
            cycles = 4;

            break;
        case 0xB4:
            // LDY dp, X  Direct Page Indexed, X (2 bytes)
            cycles = 4;

            break;
        default:
            break;
    }
    
    this->setRegisterY(registerY);
    if(registerY == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    if((registerY) & STATUS_NEGATIVE)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_NEGATIVE);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_NEGATIVE);
    return cycles;
}

int CPU::LDA_i(uint8_t opcode) {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_ACCUM_WIDTH) >> 5);
    uint8_t loadData;
    uint16_t registerA; 
    int cycles;

    if(halfWidth)
    {
        loadData = this->getProgramByte();
        registerA = loadData;
        cycles = 2;
    }
    else {
        // load two bytes into register A, low half first
        loadData = this->getProgramByte();
        registerA = loadData;
        loadData = this->getProgramByte();
        registerA += loadData << 8;
        cycles = 3;
    };
    
    this->setRegisterA(registerA);
    if(registerA == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    if((registerA) & CPU::STATUS_NEGATIVE)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_NEGATIVE);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_NEGATIVE);
    
    return cycles;
}

// todo: verify logic
int CPU::STA(uint8_t opcode) {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_ACCUM_WIDTH) >> 5);
    int cycles;
    switch(opcode) {
        case 0x81:
            // STA (dp, X)  Direct Page Indirect, X (2 bytes)
            cycles = 6;

            break;
        case 0x83:
            // STA sr, S  Stack Relative (2 bytes)
            cycles = 4;

            break;
        case 0x85:
            // STA dp  Direct Page (2 bytes)
            cycles = 3;

            break;
        case 0x87:
            // STA [dp]  Direct Page Indirect Long (2 bytes)
            cycles = 6;

            break;
        case 0x8D:
        {
            // STA addr  Absolute (3 bytes)
            cycles = 4;
            uint16_t addr = this->getProgramByte();
            addr += this->getProgramByte() << 8;
            this->setRegisterPC(this->getRegisterPC()+1);

            if(halfWidth) {
                this->getRAM()->write(addr, (uint8_t)(this->getRegisterA() << 8 >> 8));
            }
            else {
                cycles = 5;
                uint8_t higher = (uint8_t)(this->getRegisterA() >> 8);
                uint8_t lower = (uint8_t)( (this->getRegisterA() << 8) >> 8 );
                this->getRAM()->write(addr, lower);
                this->getRAM()->write(addr+1, higher);
            }

            break;
        }
        case 0x8F:
            // STA long  Absolute Long (4 bytes)
            cycles = 5;

            break;
        case 0x91:
            // STA (dp), Y  DP Indirect Indexed, Y (2 bytes)
            cycles = 6;

            break;
        case 0x92:
            // STA (dp)  Direct Page Indirect (4 bytes)
            cycles = 5;

            break;
        case 0x93:
            // STA (sr, S)  SR Indirect Indexed, Y (2 bytes)
            cycles = 7;

            break;
        case 0x95:
            // STA dp, X  Direct Page Indexed, X (2 bytes)
            cycles = 4;

            break;
        case 0x97:
            // STA [dp], Y  DP Indirect Long Indexed, Y (2 bytes)
            cycles = 4;

            break;
        case 0x99:
        {
            // STA addr, Y  Absolute Indexed, Y (3 bytes)
            cycles = 5;

            uint16_t data = this->getProgramByte();
            data += this->getProgramByte();

            uint16_t addr = data + this->getRegisterY();

            if(halfWidth) {
                this->getRAM()->write(addr, (uint8_t)(this->getRegisterA() << 8 >> 8));
            }
            else {
                cycles = 6;
                uint8_t higher = (uint8_t)(this->getRegisterA() >> 8);
                uint8_t lower = (uint8_t)( (this->getRegisterA() << 8) >> 8 );
                this->getRAM()->write(addr, lower);
                this->getRAM()->write(addr+1, higher);
            }
            break;
        }
        case 0x9D:
        {
            // STA addr, X  Absolute Indexed, X (3 bytes)
            cycles = 5;

            uint16_t data = this->getProgramByte();
            data += this->getProgramByte();

            uint16_t addr = data + this->getRegisterX();

            if(halfWidth) {
                this->getRAM()->write(addr, (uint8_t)(this->getRegisterA() << 8 >> 8));
            }
            else {
                cycles = 6;
                uint8_t higher = (uint8_t)(this->getRegisterA() >> 8);
                uint8_t lower = (uint8_t)( (this->getRegisterA() << 8) >> 8 );
                this->getRAM()->write(addr, lower);
                this->getRAM()->write(addr+1, higher);
            }
            break;
        }
        case 0x9F:
            // STA long, X  Absolute Long Indexed, X (4 bytes)
            cycles = 5;

            break;
        default:
            break;
    }

    return cycles;
}

int CPU::INX(uint8_t opcode) {
    this->setRegisterX(this->getRegisterX()+1);
    if(this->getRegisterX() == 0)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_ZERO);
    if((this->getRegisterX()) & CPU::STATUS_NEGATIVE)
        this->setRegisterPS(this->getRegisterPS() | CPU::STATUS_NEGATIVE);
    else
        this->setRegisterPS(this->getRegisterPS() & ~CPU::STATUS_NEGATIVE);
    return 2;
}

int CPU::CPX(uint8_t opcode) {
    bool halfWidth = (bool)( (this->getRegisterPS() & CPU::STATUS_INDEX_WIDTH) >> 4);
    uint8_t loadData;
    int cycles;
    uint16_t data;
    
    if(halfWidth)
    {
        loadData = this->getProgramByte();
        data = loadData;
        cycles = 2;
    }
    else
    {
        loadData = this->getProgramByte();
        data = loadData;
        loadData = this->getProgramByte();
        data += loadData << 8;
        cycles = 3;
    }
    
    uint8_t result = this->getRegisterX() - data;
    if((result) & CPU::STATUS_NEGATIVE)
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

int CPU::BNE(uint8_t opcode) {
    int cycles = 2;
    bool equal = (bool)( (this->getRegisterPS() & CPU::STATUS_ZERO) >> 1);
    int8_t offset = this->getProgramByte();
    if(!equal) {
        this->setRegisterPC(this->getRegisterPC() + offset);
        cycles = 3;
    }
    return cycles;
}

int CPU::TXS(uint8_t opcode) {
    this->setRegisterSP(this->getRegisterX());
    return 2;
}

int CPU::MVN(uint8_t opcode) {
    int cycles;
    uint8_t count = this->getRegisterA();
    uint8_t src_bank = this->getProgramByte();
    uint8_t src_block = this->getRegisterX();
    uint8_t dest_bank = this->getProgramByte();
    uint8_t dest_block = this->getRegisterY();

    //
    
    return cycles;
}

int CPU::MVP(uint8_t opcode) {
    int cycles;
    uint8_t count = this->getRegisterA();
    uint8_t src_bank = this->getProgramByte();
    uint8_t src_block = this->getRegisterX();
    uint8_t dest_bank = this->getProgramByte();
    uint8_t dest_block = this->getRegisterY();
    
    //

    return cycles;   
}