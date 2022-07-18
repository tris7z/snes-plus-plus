#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>



class CPU {
    private:
        std::uint16_t A; // accumulator
        std::uint16_t X; // index
        std::uint16_t Y; // index
        std::uint16_t SP; // stack pointer
        std::uint16_t DB; // data bank
        std::uint16_t DP; // direct page
        std::uint16_t PB; // program bank
        std::uint16_t PS; // processor status - negative, overflow, 8-bit A, 8-bit X/Y, decimal, IRQ disable, zero, carry
        std::uint16_t PC; // program counter

        std::unordered_map<std::uint16_t, int*> instruction_map;

    public:
        void helloWorld();
        void setRegisterA(std::uint16_t);
        void setRegisterX(std::uint16_t);
        void setRegisterY(std::uint16_t);
        void setRegisterSP(std::uint16_t);
        void setRegisterDB(std::uint16_t);
        void setRegisterDP(std::uint16_t);
        void setRegisterPB(std::uint16_t);
        void setRegisterPS(std::uint16_t);
        void setRegisterPC(std::uint16_t);
        std::uint16_t getRegisterA();
        std::uint16_t getRegisterX();
        std::uint16_t getRegisterY();
        std::uint16_t getRegisterSP();
        std::uint16_t getRegisterDB();
        std::uint16_t getRegisterDP();
        std::uint16_t getRegisterPB();
        std::uint16_t getRegisterPS();
        std::uint16_t getRegisterPC();

        void readAndExecute(std::string);
        void debugMode(); // interactive interpreter

        CPU() {
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
        }

};