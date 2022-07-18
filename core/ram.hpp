#include <cstdint>
#include <iostream>
#include <vector>

class RAM {
    private:
        std::vector<uint8_t> Memory = std::vector<uint8_t>(0xFFFFFF);

    public:
        void write(uint32_t address, uint8_t data);
        uint8_t read(uint32_t address);
};