/*  Alexander Symons | Apr 4 2023 | Assembler | preprocessor.c
 *  Preprocessor stage for UMI8 Assembler. Provides functions aiding the creation
 *  of the Macro Table, and parses program to fulfill assembler directives.
 */

#include <stdlib.h>
#include <stdio.h>

#include "preprocessor.h"
#include "structs.h"

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

// Handle a single instruction. Put results into processedProgram, provided valid macros, and the current macro if applicable
// Returns >0 if no errors
int preprocessInstruction(Program *processedProgram, MacroTable valid_macros, Instruction instruction){
    /*  Handles individual instructions in a program:
     *  - Add as is if no processing necessary
     *  - Expand macros if necessary
     *  - TODO: Define Constants if necessary
     *  - TODO: Evaluate expressions if necessary
     */
    static char *currentMacro;

    if (instruction.instructionType == keyword_to_type(".MACRO")) {
        // Handle Macro definitions

        char *macroName = instruction.operands[0].value;    // First operand is name

        // Gaurd against nested macro definitions
        if (currentMacro != NULL) {
            printf("CANNOT DEFINE MACRO INSIDE MACRO. %s FOUND INSIDE %s\n", macroName, currentMacro);
            return -1;
        }
        currentMacro = instruction.operands[0].value;

        printf("Current macro set to %s\n", currentMacro);

    } else if (instruction.instructionType == keyword_to_type(".END")) {
        // Gaurd against extra .ENDs
        if (currentMacro == NULL) {
            printf("ORPHANED .END FOUND\n");
            return -1;
        }

        printf("Exiting macro %s\n", currentMacro);
        currentMacro = NULL;
    } else if (instruction.instructionType == keyword_to_type("INVOKE_MACRO")) {
        printf("TODO: Validate macro\n");
    } else {
        // No preprocessor action necessary
        printf("No action\n");
        return addInstruction(processedProgram, instruction, 1);
    }
    
}

// Turn lexed token stream into a processed stream fit for generation
int preprocessProgram(Program *program, MacroTable macros, Program *processed){
    // TODO: Take in multiple programs like gcc and process instruction for all
    // TODO: free unused instructions

    // Run every instruction in program through preprocessor and put result in processed
    for (int i = 0; i < program->length; i++) {
        printf("Processing %d %s: ", i, keywords[program->Instructions[i].instructionType]);
        preprocessInstruction(processed, macros, program->Instructions[i]);
    }
    
    printf("Processed output contains %d entries\n",processed->length);

    return 1;
}