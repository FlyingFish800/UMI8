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

#endif