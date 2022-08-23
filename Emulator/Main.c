// Driver file for the CPU
#include "CPU.h"

CPU core;

int main(int argc, char *argv[]){
    reset(&core); // Clear ram doesnt work? RAM IS LINKED TO FUCKING MICROCODE???? Memory unsafe
    loadUCode(&core, "microcode.bin");
    core.RAM[RAM_SIZE-1] = 0xFF; // SP = FF
    core.RAM[0] = 0x21; // CALL I
    core.RAM[1] = 0xAA; // AA  (AABB)
    core.RAM[2] = 0xBB; // BB
    core.RAM[3] = 0x08; // LDA I
    core.RAM[4] = 0x42; // 42

    
    core.RAM[0xAABB] = 0x21; // CALL I
    core.RAM[0xAABC] = 0x69; // 69
    core.RAM[0xAABD] = 0x42; // 42
    core.RAM[0xAABE] = 0x22; // RET


    core.RAM[0x6942] = 0x22; // RET
    
    
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