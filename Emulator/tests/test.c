#include "../CPU.h"

#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

void exec_instruction(CPU *core){
    do {
        clock(core);
    } while (core->UStep != 0);
}

int main(int argc, char *argv[]){
    CPU core;
    reset(&core);
    loadUCode(&core, "microcode.bin");
    core.RAM[RAM_SIZE-1] = 0xFF; // SP = FF

    core.RAM[0] = 0x08; // LDA I
    core.RAM[1] = 0x42; // x42

    core.RAM[2] = 0x0A; // LDB I
    core.RAM[3] = 0x69; // x69

    core.RAM[4] = 0xB;  // LDB A

    core.RAM[5] = 0x21; // CALL I
    core.RAM[6] = 0xAA; // AA  (AABB)
    core.RAM[7] = 0xBB; // BB

    core.RAM[0xAABB] = 0x22; // RET

    core.RAM[8] = 0x08; // LDA I
    core.RAM[9] = 0x42; // 42
    core.RAM[10] = 0x0A;// LDB I
    core.RAM[11] = 0x01;// 1
    core.RAM[12] = 0x18;// ADD

    core.RAM[13] = 0x0A; // LDB I
    core.RAM[14] = 'B'; // 65
    core.RAM[15] = 0x0F; // LDM(I) B 0xFEFF
    core.RAM[16] = 0xFE; // 0x01
    core.RAM[17] = 0xFF; // 0x00

    core.RAM[18] = 0x08; // LDA I
    core.RAM[19] = 'A'; // 66
    core.RAM[20] = 0x0D; // LDM(I) A 0xFEFF
    core.RAM[21] = 0xFD; // 0x01
    core.RAM[22] = 0xFF; // 0x00

    core.RAM[0xFEED] = 0x80; // 0x00
    core.RAM[0xBEEF] = 0x08; // 0x00

    core.RAM[23] = 0x0C; // LD A M(I)
    core.RAM[24] = 0xFE;
    core.RAM[24] = 0xED;

    core.RAM[25] = 0x0E; // LD B M(I)
    core.RAM[26] = 0xBE;
    core.RAM[27] = 0xEF;


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

    printf("%sTesting CALL I: ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xFFFF] == 0xFD &&core.RAM[0xFFFE] == 0x00 && core.RAM[0xFFFD] == 0x06 && core.PCHI == 0xAA && core.PCLO == 0xBB) printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}

    printf("%sTesting RET: ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xFFFF] == 0xFF && core.PCHI == 0x00 && core.PCLO == 8) printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}

    exec_instruction(&core);
    exec_instruction(&core);

    printf("%sTesting ADD: ", RESET);
    exec_instruction(&core);
    if (core.A == 0x43) printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}

    exec_instruction(&core);

    printf("%sTesting LDM(I) B: ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xFEFF] == 'B') printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}
    
    exec_instruction(&core);

    printf("%sTesting LDM(I) A: ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xFDFF] == 'A') printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}

    printf("%sTesting LD A M(I): ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xFEED] == 0x80) printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}

    printf("%sTesting LD B M(I): ", RESET);
    exec_instruction(&core);
    if (core.RAM[0xBEEF] == 0x08) printf("%s SUCCESS\n", GREEN);
    else {printf("%s FAILED.\n", RED); coreDump(&core);}


    return 0;
}