#include "core.hpp"

Core::Core(std::string rom_path) {
    // initialize memory
    this->ram = new RAM();
    
    if(rom_path == "debug"
        || rom_path == "test")
    {
            this->cpu = new CPU(this->getRAM(), rom_path);
            return;
    }
    else if(rom_path == "memdump")
    {
            this->loadROM("roms/SNES_Test_Program.sfc");
            this->getRAM()->memoryDump();
            return;
    }

    this->loadROM(rom_path);
    // begin readAndExecute
    this->cpu = new CPU(this->getRAM());
}

RAM* Core::getRAM() {
    return this->ram;
}


// todo: rom needs to be written to specific locations depending on mapping mode
void Core::loadROM(std::string rom_path) {
    std::ifstream rom;
    rom.open(rom_path, std::ios::binary);

    rom.seekg(0, std::ios::end);
    uint32_t romSize = rom.tellg();
    uint16_t romSizeKb = romSize / 1024;
    rom.seekg(0, std::ios::beg);

    // todo: find the mapping mode from header data
    //uint16_t snes_header_base = 0xffff;
    // for now, assume LoROM
    this->mapping_mode = LoROM;

    if(this->mapping_mode == LoROM)
    {
        uint8_t bank = 0x80;
        uint8_t shadow_bank = 0;
        uint8_t chunks_before_mirror = romSizeKb / 32;
        // todo: understand what the hell im doing here
        // it might just work
        // https://emudev.de/q00-snes/memory-mapping-the-tale-of-lorom-and-mmio/
        for(uint32_t i = 0; i < romSize; i++)
        {
            for(uint8_t mirror = 0; mirror < (0x80 / chunks_before_mirror); mirror++)
            {
	                //  (Q3-Q4) 32k (0x8000) consecutive chunks to banks 0x80-0xff, upper halfs (0x8000-0xffff)
	                if ( (bank + (i / 0x8000) + (mirror * chunks_before_mirror)) < 0xff ) {
                        rom.seekg(i, std::ios::beg);
                        uint8_t romByte = rom.get();
	                    this->getRAM()->write( ((bank + (i / 0x8000) + (mirror * chunks_before_mirror)) << 16) | ( 0x8000 + (i % 0x8000) ), romByte );
	                }

	                //  (Q1-Q2) shadowing to banks 0x00-0x7d, except WRAM (bank 0x7e/0x7f)
	                if ((shadow_bank + (i / 0x8000) + (mirror * chunks_before_mirror)) < 0x7e) {
                        rom.seekg(i, std::ios::beg);
                        uint8_t romByte = rom.get();
	                    this->getRAM()->write( ((shadow_bank + (i / 0x8000) + (mirror * chunks_before_mirror)) << 16) | ( 0x8000 + (i % 0x8000) ), romByte );
	                }
            }
        }
    }

/*
    uint32_t location = 0x8000;
    char romByte;
    while (!rom.eof()) {
        if(rom.eof()) break;
        rom.read(&romByte, 1);
        this->ram->write(location, romByte);
        location++;
    }
*/
    rom.close();
}