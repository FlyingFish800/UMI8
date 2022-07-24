// Driver file for the CPU
#include "CPU.h"

CPU core;

int main(int argc, char *argv[]){
    reset(&core);
    loadUCode(&core, "microcode.bin");
    core.RAM[0] = 0x8;  // LDA
    core.RAM[1] = 0x42; // 42
    coreDump(&core);
    for (size_t i = 0; i < 7; i++)
    {
        // UCode index and step mismatch because clock updates step at end
        clock(&core);
        coreDump(&core);
    }
    
} // end main