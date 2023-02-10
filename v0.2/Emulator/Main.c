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
    
    loadRam(&core, "../Assembler/a.bin");

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
