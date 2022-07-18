#include "core/cpu.hpp"

int main(int argc, char *argv[]) {
    if(argc == 1)
    {
        std::cout << "Please enter rom path" << std::endl;
    }
    else
    {
        CPU cpu(argv[1]);
    }
}
