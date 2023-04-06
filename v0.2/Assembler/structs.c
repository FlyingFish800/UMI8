#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "structs.h"

// Take in the name of a token and convert it to its respective type id using its index
int keyword_to_type(char *token){
    for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
        if (strcmp(keywords[i], token) == 0) return i;
    }
    return -1;
}

// Push an operand to the Instruction's stack of operands. Return 1 to continue
int addOperand(Instruction *i, Operand o){
    i->operandsLength += 1;
    i->operands = realloc(i->operands, sizeof(Operand)*i->operandsLength);
    if (i->operands == NULL) {printf("COULDNT REALLOC OPERANDS"); return 0;}
    i->operands[i->operandsLength-1] = o;
    return 1;
}

// Push an instruction to the program (stack of instructions). Return 1 to continue
int addInstruction(Program *p, Instruction i, int returnCode){
    p->length += 1;
    p->Instructions = realloc(p->Instructions, sizeof(Instruction)*p->length);
    if (p->Instructions == NULL) {printf("COULDNT REALLOC INSTRUCTIONS"); return 0;}
    p->Instructions[p->length-1] = i;
    return returnCode;
}