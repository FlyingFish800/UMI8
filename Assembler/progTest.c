#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

void addInstruction(Program *p, Instruction i){
    p->length += 1;
    p->Instructions = realloc(p->Instructions, sizeof(Instruction)*p->length);
    p->Instructions[p->length-1] = i;
    if (p->Instructions == NULL) perror("COULDNT REALLOC");
}

void addOperand(Instruction *i, Operand o){
    i->operandsLength += 1;
    i->operands = realloc(i->operands, sizeof(Operand)*i->operandsLength);
    i->operands[i->operandsLength-1] = o;
    if (i->operands == NULL) perror("COULDNT REALLOC OPERANDS");
}

void passthru(Program *p, Instruction i){
    addInstruction(p, i);
}

int main(){
    /*Program prog;
    prog.length = 0;
    prog.Instructions = malloc(0);

    Instruction in;
    in.instructionType = RET;
    in.operands = NULL;

    addInstruction(&prog, in);
    addInstruction(&prog, in);
    addInstruction(&prog, in);
    addInstruction(&prog, in);
    addInstruction(&prog, in);
    addInstruction(&prog, in);

    for (int i = 0; i < prog.length; i++) printf("%d\n",prog.Instructions[i].instructionType);
    printf("%d\n",prog.length);

    free(prog.Instructions);*/

    Instruction instruction;
    instruction.operandsLength = 0;
    instruction.operands = malloc(0);

    Operand operand;
    operand.value = "Hello!";

    addOperand(&instruction, operand);
    operand.value = "World!";
    addOperand(&instruction, operand);

    for (int i = 0; i < instruction.operandsLength; i++) printf("%s\n",instruction.operands[i].value);
    printf("%d\n",instruction.operandsLength);

    free(instruction.operands);
    return 0;
}