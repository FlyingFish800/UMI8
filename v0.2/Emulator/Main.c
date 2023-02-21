// Alexander Symons | Aug 6 2022 | Emulator | Main.c
// Driver file for the CPU
// Inputs: microcode binary
#define DEBUG
#include "CPU.h"
#include "IODevices/terminal.h"

CPU core;
IODevice dummy;

//int number(){
//    return 0x42;
//}

int main(int argc, char *argv[]){
    dummy.address = 0xFEFF;
    dummy.readDevice = recieveChar;
    dummy.writeDevice = sendChar;


    reset(&core);
    
    loadUCode(&core, "microcode.bin");

    registerDevice(&core, &dummy);
    
    loadRam(&core, "../Assembler/a.bin");
    core.RAM[0xFFFF] = 0xFE;

    /*core.RAM[0] = 0x08; // LD A, 0x42
    core.RAM[1] = 0x42;
    core.RAM[2] = 0x3; // PUSH A
    core.RAM[3] = 0x08; // LD A, 0x42
    core.RAM[4] = 0x69;
    core.RAM[5] = 0x4; // POP A

    
    core.RAM[0x0] = 0x1; // CALL 0xABCD
    core.RAM[0x1] = 0xCD;
    core.RAM[0x2] = 0xAB;

    core.RAM[0xABCD] = 0x1; // CALL 0xABCD
    core.RAM[0xABCE] = 0x69;
    core.RAM[0xABCF] = 0x42;
    core.RAM[0xABD0] = 0x2; // RET

    core.RAM[0x4269] = 0x2; // RET*/


    char c = 0x0;
    while (c != 'q') {
        // UCode index and step mismatch because clock updates step at end
        #ifdef DEBUG
        c = getchar();
        #endif

        if (c == 'd'){
            for (int i = 0; i < RAM_SIZE; i++){
                if (core.RAM[i] != 0) printf("0x%x: 0x%x\n", i, core.RAM[i]);
            }
        } else clock(&core);

        #ifdef DEBUG
        coreDump(&core);
        #endif
    }
    
} // end main
