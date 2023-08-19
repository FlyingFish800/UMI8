#include "../CPU.h"

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

void exec_instruction(CPU *core){
    do {
        clock_core(core);
    } while (core->UStep != 0);
}

void debug_instruction(CPU *core){
    do {
        getchar();
        clock_core(core);
        coreDump(core);
    } while (core->UStep != 0);
}

int main(int argc, char *argv[]){
    CPU core;
    reset(&core);
    loadUCode(&core, "microcode.bin");
    core.RAM[RAM_SIZE-1] = 0xFE; // SP = FF

    core.RAM[0] = 0x08; // LDA I
    core.RAM[1] = 0x42; // x42

    core.RAM[2] = 0x0A; // LDB I
    core.RAM[3] = 0x69; // x69


    printf("%sTesting LDA I: ", RESET);
    exec_instruction(&core);
    if (core.A != 0x42) printf("%s FAILED. GOT 0x%x\n", RED, core.A);
    else printf("%s SUCCESS\n", GREEN);

    printf("%sTesting LDB I: ", RESET);
    exec_instruction(&core);
    if (core.B != 0x69) printf("%s FAILED. GOT 0x%x\n", RED, core.B);
    else printf("%s SUCCESS\n", GREEN);

    printf("%sTesting LDB A: ", RESET);
    exec_instruction(&core);
    if (core.B != 0x42) printf("%s FAILED. GOT 0x%x\n", RED, core.B);
    else printf("%s SUCCESS\n", GREEN);


    return 0;
}