#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <unordered_map>
// should this be abstracted later?
#include "ram.hpp"

class CPU {
    private:
        std::uint16_t A; // accumulator
        std::uint16_t X; // index
        std::uint16_t Y; // index
        std::uint16_t SP; // stack pointer
        std::uint16_t DB; // data bank
        std::uint16_t DP; // direct page
        std::uint16_t PB; // program bank
        std::uint8_t PS; // processor status - negative, overflow, 8-bit A, 8-bit X/Y, decimal, IRQ disable, zero, carry
        std::uint16_t PC; // program counter

    public:
        const uint8_t STATUS_CARRY = 1;
        const uint8_t STATUS_ZERO = (1 << 1);
        const uint8_t STATUS_IRQ_DISABLE = (1 << 2);
        const uint8_t STATUS_DECIMAL = (1 << 3);
        const uint8_t STATUS_INDEX_WIDTH = (1 << 4);
        const uint8_t STATUS_ACCUM_WIDTH = (1 << 5);
        const uint8_t STATUS_OVERFLOW = (1 << 6);
        const uint8_t STATUS_NEGATIVE = (1 << 7);
         
        typedef int (CPU::*instruction)(); // returns cycles
        std::unordered_map<std::uint16_t, instruction> IMap = {
//            {0x18, (instruction)&CPU::test}
            {0x18, (instruction)&CPU::CLC},
            {0xFB, (instruction)&CPU::XCE},
            {0xC2, (instruction)&CPU::REP},
            {0xE2, (instruction)&CPU::SEP},
            {0xA2, (instruction)&CPU::LDX_i},
            {0xA9, (instruction)&CPU::LDA_i},
            {0x9D, (instruction)&CPU::STA_AiX},
            {0xE8, (instruction)&CPU::INX},
            {0xE0, (instruction)&CPU::CPX},
            {0xD0, (instruction)&CPU::BNE},
            {0x9A, (instruction)&CPU::TXS},
            {0x8D, (instruction)&CPU::STA_addr},

        };
        RAM ram;
        
        void setRegisterA(std::uint16_t);
        void setRegisterX(std::uint16_t);
        void setRegisterY(std::uint16_t);
        void setRegisterSP(std::uint16_t);
        void setRegisterDB(std::uint16_t);
        void setRegisterDP(std::uint16_t);
        void setRegisterPB(std::uint16_t);
        void setRegisterPS(std::uint8_t);
        void setRegisterPC(std::uint16_t);
        std::uint16_t getRegisterA();
        std::uint16_t getRegisterX();
        std::uint16_t getRegisterY();
        std::uint16_t getRegisterSP();
        std::uint16_t getRegisterDB();
        std::uint16_t getRegisterDP();
        std::uint16_t getRegisterPB();
        std::uint8_t getRegisterPS();
        std::uint16_t getRegisterPC();

        void readAndExecute();
        void loadROM(std::string);

        // debug and helpers
        void debugMode(); // interactive interpreter
        void dump_registers();
        void printHex(uint16_t);
        std::vector<uint8_t> HexToBytes(const std::string&);
        int test(std::string);
        void helloWorld();

        // instruction methods
        int CLC();
        int XCE();
        int REP();
        int SEP();
        int LDX_i();
        int LDA_i();
        int STA_AiX();
        int STA_addr();
        int INX();
        int CPX();
        int BNE();
        int TXS();

        CPU(std::string rom_path) {           
            // this could be replaced with an instruction function later
            setRegisterA(0);
            setRegisterX(0);
            setRegisterY(0);
            setRegisterSP(0);
            setRegisterDB(0);
            setRegisterDP(0);
            setRegisterPB(0);
            setRegisterPS(0);
            setRegisterPC(0);

            if(rom_path == "debug")
                debugMode();
                
            loadROM(rom_path);
            readAndExecute();
        }

};