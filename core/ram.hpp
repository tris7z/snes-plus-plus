#ifndef _SPP_RAM_
#define _SPP_RAM_

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <unordered_map>

/*********
 * notes
 * 
 * quadrant model:
 *   q1: bank $00-$3f
 *   q2: $40-$7f
 *   q3: $80-$Bf
 *   q4: $C0-$ff
 *     bottom: $0000-$7fff
 *     top: $8000-$ffff
 * 
 * bank: most significant byte of an address XX0000-XXffff
 * page: smallest mappable unit - 256 bytes
 *   all pages end at $FF
 * 256 bytes make a page, 256 pages make a bank, 256 banks cover the address space
 *   
 * memory map depends on rom type (mapping mode): LoROM or HiROM (and others)
 *   rom type is found at byte $15 (learn more, set state at init?)
 * type-independent regions:
 *   workRAM: 128kb or 2 banks, $7e00000-7fffff
 *     first 32 pages (7e0000-7e1fff) are mirrored, can be accessed from any bank in q1 or q3
 *       this is a hardware quirk(?) feature?
 *   external chip registers: mirrored in q1 and q3
 *     ppu registers: page $21
 *     cpu registers: pages $42-$43
 *     joypad: $4016-$4017
 *   all other regions free to use
 *     aux: unused bottom half of q1 and q3
 *     ROMSEL: top of q1/q3 and all of q2 (except wram region) and q4
 *       region for rom and ram, technically a hardware trick to prevent write conflicts
 *         decoder chips allow precise control of all onboard hardware
 * mapping modes:
 *   LoROM (mode 20):
 *     "the first half bank of the rom is mapped to the top half of bank $80" $808000-$80ffff
 *     the second half of the first bank maps to $818000-$81ffff
 *     continue until rom end, up to 4MB
 *       if space remains, top of q3/q4 will mirror rom
 *       bottom of q4 (up to $f0) also mirrors
 *     SRAM: if present? bottom half of bank $F0: $f00000-$f07fff (32kb)
 *       map any more sram to bottom half of following banks (f1, f2...)
 *       leftover space will be mirror
 *     q1 and q2 will mirror q3 and q4
 *       if rom is large, not all is accessible from q2
 *   HiROM (mode 21):
 *     rom maps from bank $c0 to $ff (4MB cap)
 *     SRAM: starts at $b06000(?)- $b07fff (8kb)
 *       more sram maps the same from bank $b0-$bf
 *       "may or may not be mirrored across $80-$af"
 *     leftover space mirrors
 *     q1/q2 mirror q3/q4
 * 
 * snes header: stored at the end of bank 0 offset from rom ($00FFFF up)
 *   stores name, interrupt vectors(?), version, more...
 *   only interrupt vectors affect the code, other info is metadata (maybe mapping mode)
 *   $FFD5 holds mapping mode (M) / speed switch (S): SSSS MMMM 
 * 
 * interrupt vectors: set of 12 (10 used) 16-bit pointers that define interrupt routines supplied by the rom
 *   half of them are native, half are for emulation mode
 *   native e4-ef, emulation f4-ff
 *   COP: call this routine when instruction COP is executed
 *   BRK (native only): routine for BRK instruction
 *   ABORT (unused)
 *   NMI: non-maskable interrupt something to do with vertical blanking (??)
 *   RESET (only used in emulation mode)
 *   IRQ: dunno this one either
 *   LoROM: vectors begin at offset $7fe4 (rom begins at $80000, 24 bytes up from $7fff)
 *   HiROM: offset $ffe4 (not sure ?)
 *       
 * 
 */

class RAM {
    private:
        std::vector<uint8_t> Memory = std::vector<uint8_t>(0xFFFFFF);

    public:
        void write(uint32_t address, uint8_t data);
        uint8_t read(uint32_t address);
        void memoryDump();
};

#endif