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

    core.RAM[2] = 0x28; // LDC I
    core.RAM[3] = 0x69; // x69

    core.RAM[4] = 0x00; // NOP

    core.RAM[5] = 0x02; // PPC

    core.RAM[6] = 0x10; // JP I
    core.RAM[7] = 0x34; // Low x34
    core.RAM[8] = 0x12; // High x12
    
    core.RAM[0x1234] = 0x3; // RET

    printf("%sTesting LD A, I: ", RESET);
    exec_instruction(&core);
    if (core.A != 0x42) printf("%s FAILED. GOT 0x%x\n", RED, core.A);
    else printf("%s SUCCESS\n", GREEN);

    printf("%sTesting LD C, I: ", RESET);
    exec_instruction(&core);
    if (core.C != 0x69) printf("%s FAILED. GOT 0x%x\n", RED, core.C);
    else printf("%s SUCCESS\n", GREEN);

    CPU core_save;
    copy_core(&core, &core_save);

    printf("%sTesting NOP: ", RESET);
    exec_instruction(&core);
    if (check_cores(&core, &core_save, CHECK_EQL) != 0) printf("%s FAILED. GOT 0x%x\n", RED, check_cores(&core, &core_save, CHECK_EQL));
    else printf("%s SUCCESS\n", GREEN);

    printf("%sTesting PPC: ", RESET);
    exec_instruction(&core);
    if (!((core.RAM[0xFFFE] == 6) && (core.RAM[0xFFFD] == 0))) printf("%s FAILED. GOT 0x%02x%02x\n", RED, core.RAM[0xFFFE], core.RAM[0xFFFD]);
    else printf("%s SUCCESS\n", GREEN);

    printf("%sTesting JP I: ", RESET);
    exec_instruction(&core);
    if (!((core.PCHI == 0x12) && (core.PCLO == 0x34))) printf("%s FAILED. GOT 0x%02x%02x\n", RED, core.PCHI, core.PCLO);
    else printf("%s SUCCESS\n", GREEN);

    //debug_instruction(&core);

    printf("%sTesting RET: ", RESET);
    exec_instruction(&core);
    if (!((core.PCHI == 0) && (core.PCLO == 9))) printf("%s FAILED. GOT 0x%02x%02x\n", RED, core.PCHI, core.PCLO);
    else printf("%s SUCCESS\n", GREEN);



    return 0;
}