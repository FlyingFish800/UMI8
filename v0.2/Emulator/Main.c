// Alexander Symons | Aug 6 2022 | Emulator | Main.c
// Driver file for the CPU
// Inputs: microcode binary
#define DEBUG
#include "CPU.h"
#include "IODevices/terminal.h"

CPU core;
IODevice dummy;

int sixnine(){
    return 0x69;
}

int main(int argc, char *argv[]){
    reset(&core);
    
    loadUCode(&core, "microcode.bin");


    core.RAM[0] = 0x10; // JP I
    core.RAM[1] = 0x01; // 0x1001
    core.RAM[2] = 0x10;


    core.RAM[0x1001] = 0x28; // LD A I
    core.RAM[0x1002] = 0x42; // 0x42
    core.RAM[0x1003] = 0x38; // ADD I
    core.RAM[0x1004] = 0x69; // 0x69
    

    char c = 0x0;
    while (c != 'q') {
        // UCode index and step mismatch because clock updates step at end
        #ifdef DEBUG
        c = getchar();
        #endif

        clock(&core);

        #ifdef DEBUG
        coreDump(&core);
        #endif
    }
    
} // end main
