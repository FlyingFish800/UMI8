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

int getMacroOperandType(char *operand){
    if(operand[0] == '_') {
        printf("ABS_LABEL");
        return ABSOLUTE_LABEL;
    } else if(operand[0] == 'A' || operand[0] == 'C') {
        printf("REG");
        return REGISTER;
    } else if(operand[0] == '#') {
        printf("IMM");
        return IMMEDIATE;
    } else if(operand[0] == '[') {
        if(operand[1] == '[') { // Double memory lookup [[#0xCAFE]]
            printf("REL");
            return RELATIVE;
        }  else if(operand[1] == '_') { // Memory lookup of label [_pointer]
            printf("REL_LABEL");
            return RELATIVE_LABEL;
        } else { // Single lookup of immediate [#0xCAFE]
            printf("ABS");
            return ABSOLUTE;
        }
    } else if(operand[0] == '"') {
        printf("STR");
        return STRING;
    } else {
        printf("UNIMPLEMENTED/INVALID OPERAND <%s> IN PARSING\n", operand);
        return -1;
    }
}

// Check that the current instruction fits a valid macro prototype
int checkMacroValid(MacroTable valid_macros, Instruction instruction){
    // Operand 0 is name
    char *instructionName = instruction.operands[0].value;

    // Check each macro
    for (int i = 0; i < valid_macros.length; i++){
        // First check to see if they have the same operand count
        // -1 for instruction having identifier as #1
        Macro testMacro = valid_macros.macros[i];
        if (testMacro.operandsLength != instruction.operandsLength-1) return 0;
        printf("Same len ");

        // Get name of macro being checked, continue if it isn't being invoked
        char *currentMacroName = testMacro.identifier;
        if (strcmp(currentMacroName, instructionName) != 0) continue;
        printf("Same id ");

        // Check operand types. Labels fine as adresses and vice versa
        for (int o = 1; o < instruction.operandsLength; o++){
            // Temp variables so these if statements aren't too horendous...
            Operand macroOp = testMacro.operands[o - 1]; // -1 to account for skipped identifier
            Operand insOp = instruction.operands[o];

            // Parse the operand type of the macro arg
            char macroOpMode = getMacroOperandType(macroOp.value);
            if (macroOpMode <= 0) return 0;

            // TODO: how to parse macro args??? Look at my old code
            printf(" %d vs %d ", macroOpMode, insOp.accesingMode);

            // Check if modes are the same or equivalent
            if (macroOpMode == insOp.accesingMode) continue;
            else if(macroOpMode == ABSOLUTE && insOp.accesingMode == ABSOLUTE_LABEL) continue;
            else if(macroOpMode == ABSOLUTE_LABEL && insOp.accesingMode == ABSOLUTE) continue;
            else if(macroOpMode == RELATIVE && insOp.accesingMode == RELATIVE_LABEL) continue;
            else if(macroOpMode == RELATIVE_LABEL && insOp.accesingMode == RELATIVE) continue;
            else return 0; // Invalid
        }

        // Valid!
        return 1;
    }

    // Shouldn't happen, lexer should error if it doesn't exist
    return 0;
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
        // Gaurd statement to validate macro prototype
        if(!checkMacroValid(valid_macros, instruction)) {
            printf("NOT FOUND\n");
            return -1;
        }
        printf("Found!\n");
    } else {
        // No preprocessor action necessary
        printf("No preprocessor action\n");
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