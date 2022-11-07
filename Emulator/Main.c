// Alexander Symons | Aug 6 2022 | Emulator | Main.c
// Driver file for the CPU
// Inputs: microcode binaru
#include "CPU.h"

CPU core;
IODevice dummy;

int sixnine(){
    return 0x69;
}

int main(int argc, char *argv[]){
    dummy.address = 2;
    dummy.readDevice = sixnine;

    reset(&core);
    loadUCode(&core, "microcode.bin");
    core.RAM[RAM_SIZE-1] = 0xFF; // SP = FF
    registerDevice(&core,&dummy);

    core.RAM[1] = 0x08;
    core.RAM[2] = 0x42;

    
    /*core.RAM[0] = 0x21; // CALL I
    core.RAM[1] = 0xAA; // AA  (AABB)
    core.RAM[2] = 0xBB; // BB
    core.RAM[3] = 0x08; // LDA I
    core.RAM[4] = 0x42; // 42

    
    core.RAM[0xAABB] = 0x21; // CALL I
    core.RAM[0xAABC] = 0x69; // 69
    core.RAM[0xAABD] = 0x42; // 42
    core.RAM[0xAABE] = 0x22; // RET


    core.RAM[0x6942] = 0x22; // RET
    
    /*core.RAM[0] = 0x0A; // LDB I
    core.RAM[1] = 0x42; // 42
    core.RAM[2] = 0x23; // PUSH B
    core.RAM[3] = 0x0A; // LDB I
    core.RAM[4] = 0x69; // 69
    core.RAM[5] = 0x23; // PUSH B
    core.RAM[6] = 0x22; // ret

    core.RAM[0x426B] = 0x8; // LDA I
    core.RAM[0x426C] = 0x42; // 42
    coreDump(&core);*/

    char c = 0x0;
    while (c != 'q') {
        // UCode index and step mismatch because clock updates step at end
        c = getchar();
        clock(&core);
        coreDump(&core);
    }
    
} // end main
