// Driver file for the CPU
#include "CPU.h"

CPU core;

int main(int argc, char *argv[]){
    reset(&core);
    loadUCode(&core, "microcode.bin");
    core.RAM[0] = 0x10; // JP I
    core.RAM[1] = 0xAA; // AA  (AABB)
    core.RAM[2] = 0xBB; // BB
    
    
    
    // Problems: First AO MIH might be going to MIL?
    // Need workling ALU ;p
    coreDump(&core);
    while (1) {
        // UCode index and step mismatch because clock updates step at end
        getchar();
        clock(&core);
        coreDump(&core);
    }
    
} // end main