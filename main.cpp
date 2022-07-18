#include "core/cpu.hpp"

int main(int argc, char *argv[]) {
    CPU cpu;

    if(argc == 1)
        cpu.helloWorld();
    else
        cpu.readAndExecute(argv[1]);
}
