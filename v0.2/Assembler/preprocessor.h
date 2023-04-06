#ifndef PREPROCESSOR
#define PREPROCESSOR

#include "structs.h"

// Macro to be expanded into body
typedef struct Macro {
    char *identifier;
    int operandsLength;
    Operand *operands;
    Program body;
} Macro;

// MacroTable struct that contains all macros as a queue
typedef struct MacroTable {
    int length;
    Macro *macros;
} MacroTable;

int registerMacroOperand(Macro *m, Operand o);

int registerMacro(MacroTable *mt, Macro m, int returnCode);

int preprocessProgram(Program *program, MacroTable macros, Program *processed);

#endif