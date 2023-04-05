#include "preprocessor.h"
#include <stdlib.h>
#include <stdio.h>

// Push a macro to the Macro table to register it as a valid macro
int registerMacro(MacroTable *mt, Macro m, int returnCode){
    mt->length += 1;
    printf("REGISTERING MACRO %s #%d\n", m.identifier, mt->length);
    mt->macros = realloc(mt->macros, sizeof(Macro)*mt->length);
    if (mt->macros == NULL) {perror("COULDNT REALLOC MACROS"); return 0;}
    mt->macros[mt->length-1] = m;
    return returnCode;
}

// Push a required operand to the Macro
int registerMacroOperand(Macro *m, Operand o){
    m->operandsLength += 1;
    m->operands = realloc(m->operands, sizeof(Operand)*m->operandsLength);
    if (m->operands == NULL) {perror("COULDNT REALLOC OPERANDS"); return 0;}
    m->operands[m->operandsLength-1] = o;
    return 1;
}