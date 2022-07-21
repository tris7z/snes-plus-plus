#ifndef _SPP_CPU_
#define _SPP_CPU_

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

#include "ram.hpp"

class CPU {
    private:
        RAM* ram;

        uint16_t A; // accumulator
        uint16_t X; // index
        uint16_t Y; // index
        uint16_t SP; // stack pointer
        uint16_t DB; // data bank
        uint16_t DP; // direct page
        uint16_t PB; // program bank
        uint8_t PS; // processor status - negative, overflow, 8-bit A, 8-bit X/Y, decimal, IRQ disable, zero, carry
        uint16_t PC; // program counter

    public:
        CPU(RAM*);
        CPU(RAM*, std::string);

        const uint8_t STATUS_CARRY = 1;
        const uint8_t STATUS_ZERO = (1 << 1);
        const uint8_t STATUS_IRQ_DISABLE = (1 << 2);
        const uint8_t STATUS_DECIMAL = (1 << 3);
        const uint8_t STATUS_INDEX_WIDTH = (1 << 4);
        const uint8_t STATUS_ACCUM_WIDTH = (1 << 5);
        const uint8_t STATUS_OVERFLOW = (1 << 6);
        const uint8_t STATUS_NEGATIVE = (1 << 7);
         
        typedef int (CPU::*instruction)(uint8_t); // returns cycles
        std::unordered_map<std::uint16_t, instruction> IMap = {
//            {0x18, (instruction)&CPU::test}
            {0x18, (instruction)&CPU::CLC},
            {0xFB, (instruction)&CPU::XCE},
            {0xC2, (instruction)&CPU::REP},
            {0xE2, (instruction)&CPU::SEP},
            {0xA2, (instruction)&CPU::LDX},
            {0xA9, (instruction)&CPU::LDA_i},
            {0x8D, (instruction)&CPU::STA},
            {0x9D, (instruction)&CPU::STA},
            {0x99, (instruction)&CPU::STA},
            {0xE8, (instruction)&CPU::INX},
            {0xE0, (instruction)&CPU::CPX},
            {0xD0, (instruction)&CPU::BNE},
            {0x9A, (instruction)&CPU::TXS},
            {0xA0, (instruction)&CPU::LDY},
            //{0x54, (instruction)&CPU::MVN},
            //{0x44, (instruction)&CPU::MVP},

        };

        void setRAM(RAM* ram); 
        void setRegisterA(uint16_t);
        void setRegisterX(uint16_t);
        void setRegisterY(uint16_t);
        void setRegisterSP(uint16_t);
        void setRegisterDB(uint16_t);
        void setRegisterDP(uint16_t);
        void setRegisterPB(uint16_t);
        void setRegisterPS(uint8_t);
        void setRegisterPC(uint16_t);
        void zeroRegisters();
        RAM* getRAM();
        uint16_t getRegisterA();
        uint16_t getRegisterX();
        uint16_t getRegisterY();
        uint16_t getRegisterSP();
        uint16_t getRegisterDB();
        uint16_t getRegisterDP();
        uint16_t getRegisterPB();
        uint8_t getRegisterPS();
        uint16_t getRegisterPC();

        uint8_t getProgramByte();

        void readAndExecute();
        void loadROM(std::string);

        // debug and helpers
        void debugMode(); // interactive interpreter
        void dump_registers();
        void printHex(uint16_t);
        std::vector<uint8_t> HexToBytes(const std::string&);
        void test();
        void helloWorld();

        // instruction methods
        // TODO? maybe we should double check them all when they're done
        int CLC(uint8_t);
        int XCE(uint8_t);
        int REP(uint8_t);
        int SEP(uint8_t);
        int LDX(uint8_t);
        int LDA_i(uint8_t);
        int STA(uint8_t);
        int INX(uint8_t);
        int CPX(uint8_t);
        int BNE(uint8_t);
        int TXS(uint8_t);
        int LDY(uint8_t);
        int MVN(uint8_t);
        int MVP(uint8_t);
        

};

#endif