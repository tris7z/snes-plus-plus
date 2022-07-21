#ifndef _SPP_CORE_
#define _SPP_CORE_

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "cpu.hpp"
#include "ram.hpp"

class Core {
    public:
        Core(std::string);

        enum MappingMode {
            LoROM,
            HiROM
        };

        RAM* getRAM();
        void loadROM(std::string);
    
    private:
        Core::MappingMode mapping_mode;
        RAM* ram;
        CPU* cpu;


        
    
};

#endif