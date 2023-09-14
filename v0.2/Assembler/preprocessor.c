/*  Alexander Symons | Apr 4 2023 | Assembler | preprocessor.c
 *  Preprocessor stage for UMI8 Assembler. Provides functions aiding the creation
 *  of the Macro Table, and parses program to fulfill assembler directives.
 */

#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "preprocessor.h"
#include "structs.h"

#define INCLUDE_FILE 2

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

// Unused. Arg checking left as a task for codegen to simplify macro system
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

// Check all valid macros given to find index of specified macro
// return index or -1 if invalid
int getMacroIndexFromName(MacroTable *valid_macros, char *macroName){
    // Check each macro
    for (int i = 0; i < valid_macros->length; i++){
        char *currentMacroName = valid_macros->macros[i].identifier;
        if (strcmp(currentMacroName, macroName) == 0) return i;
    }

    // Macro not found
    return -1;

}

// Check that the current instruction's arguments fit a valid macro prototype
int checkMacroValid(MacroTable *valid_macros, Instruction instruction){
    // Operand 0 is name
    char *instructionName = instruction.operands[0].value;

    // Find macro. Return error if not found
    int macroIndex = getMacroIndexFromName(valid_macros, instructionName);
    if (macroIndex == -1) return -1;
    printf("Same id ");

    // Get macro and check operands are same length
    Macro testMacro = valid_macros->macros[macroIndex];
    if (testMacro.operandsLength != instruction.operandsLength-1) return -1;
    printf("Same len ");

    // NOTE: Don't check types. This is a macro system, so just paste code w/ args
    // Check operand types. Labels fine as adresses and vice versa
    // for (int o = 1; o < instruction.operandsLength; o++){
    //     // Temp variables so these if statements aren't too horendous...
    //     Operand macroOp = testMacro.operands[o - 1]; // -1 to account for skipped identifier
    //     Operand insOp = instruction.operands[o];
    // 
    //     // Parse the operand type of the macro arg
    //     char macroOpMode = getMacroOperandType(macroOp.value);
    //     if (macroOpMode <= 0) return -1;
    // 
    //     // TODO: how to parse macro args??? Look at my old code
    //     printf(" %d vs %d ", macroOpMode, insOp.accesingMode);
    // 
    //     // Check if modes are the same or equivalent NOTE: Don't need euqivalent
    //     if (macroOpMode == insOp.accesingMode) continue;
    //     //else if(macroOpMode == ABSOLUTE && insOp.accesingMode == ABSOLUTE_LABEL) continue;
    //     //else if(macroOpMode == ABSOLUTE_LABEL && insOp.accesingMode == ABSOLUTE) continue;
    //     //else if(macroOpMode == RELATIVE && insOp.accesingMode == RELATIVE_LABEL) continue;
    //     //else if(macroOpMode == RELATIVE_LABEL && insOp.accesingMode == RELATIVE) continue;
    //     else return -1; // Invalid
    // }

    // Valid! Return index
    return macroIndex;

}

// Checks if an operand's name is from the definition of a macro
// Returns the index of name if found, otherwise -1;
int inMacroDef(Macro m, char *name){
    for (int i = 0; i < m.operandsLength; i++){
        printf(" %d:[%s]",i,m.operands[i].value);
        if (strcmp(name, m.operands[i].value) == 0) return i;
    }
    return -1;
}

// Given an instruction and a macro it is from, convert that instruction for this macro call.
// Ex:
// .macro CALL <_lbl>
//      PPC
//      JP _lbl
// .end
// CALL _subroutine
// JP _lbl would get translated to JP _subroutine, and PPC would be untouched
// Caller should error if returned instructionType is -1
Instruction createModifiedInstruction(Macro macro, Instruction protoInstruction, Instruction macroCall){
    // Create new instruction fulfilling macro
    Instruction newInstruction;
    newInstruction.operands = malloc(sizeof(Operand) * protoInstruction.operandsLength);
    newInstruction.operandsLength = protoInstruction.operandsLength;

    // Make sure macros are not recursively called. 
    // TODO: Remove?
    if ((protoInstruction.operandsLength > 0) && (protoInstruction.operands[0].accesingMode == MACRO_ID)){
        newInstruction.instructionType = -1;
        return newInstruction;
    }
    newInstruction.instructionType = protoInstruction.instructionType;

    // For each operand
    for (int i = 0; i < protoInstruction.operandsLength; i++){
        // Index into operand template in macro definition
        int protoIndex;

        // Lookup index in macro potoype, continue if not in prototype
        if ((protoIndex = inMacroDef(macro, protoInstruction.operands[i].value)) == -1) {
            // If in prototype, copy over
            newInstruction.operands[i] = protoInstruction.operands[i];
            printf(" [%s]", newInstruction.operands[i].value);
            continue;
        }

        // If from prototype, fulfill using type for the protoype, and get value
        // from the macro call (using index from prototype)
        newInstruction.operands[i].accesingMode = protoInstruction.operands[i].accesingMode;
        newInstruction.operands[i].value = macroCall.operands[i + 1].value; 
        // +1 because in macro definition the name is the first operand, but in prototype that isnt 

        printf(" %d:<%s>", protoIndex, newInstruction.operands[i].value);
    }

    return newInstruction;
}

// Handle a single instruction. Put results into processedProgram, provided valid macros, 
// and the current macro if applicable. Returns >0 if no errors
int preprocessInstruction(Program *processedProgram, MacroTable *valid_macros, Instruction instruction){
    /*  Handles individual instructions in a program:
     *  - Add as is if no processing necessary
     *  - Expand macros if necessary
     *  - TODO: Define Constants if necessary
     *  - TODO: Evaluate expressions if necessary
     */
    static int currentMacro = -1;

    if (instruction.instructionType == keyword_to_type(".MACRO")) {
        // Handle Macro definitions
        char *macroName = instruction.operands[0].value;    // First operand is name

        // Gaurd against nested macro definitions
        if (currentMacro != -1) {
            printf("CANNOT DEFINE MACRO INSIDE MACRO. %s ", macroName);
            printf("FOUND INSIDE %s\n", valid_macros->macros[currentMacro].identifier);
            return -1;
        }
        
        // Mark the current macro, exit if invalid
        if ((currentMacro = getMacroIndexFromName(valid_macros, macroName))== -1) return -1;
        valid_macros->macros[currentMacro].body.Instructions = malloc(0);
        valid_macros->macros[currentMacro].body.length = 0;
        printf("Current macro set to %s\n", macroName);

    } else if (instruction.instructionType == keyword_to_type(".END")) {
        // Gaurd against extra .ENDs
        if (currentMacro == -1) {
            printf("ORPHANED .END FOUND\n");
            return -1;
        }

        // Current macro has been fully processed
        printf("Exiting macro %s\n", valid_macros->macros[currentMacro].identifier);
        currentMacro = -1;

    }  else if (instruction.instructionType == keyword_to_type(".INCLUDE")) {
        // Don't add, just tell calling function that inlcude needs to be done
        return INCLUDE_FILE;

    } else if (instruction.instructionType == keyword_to_type("INVOKE_MACRO")) {
        int index;

        // Gaurd statement to validate macro prototype
        if((index = checkMacroValid(valid_macros, instruction)) == -1) {
            printf("NOT FOUND\n");
            return -1;
        }

        // Found macro
        printf("Found!\n");
        Macro macro = valid_macros->macros[index];

        // Create an ordered list of all macro operands:
        // Eg. for PPC _start -> ['_start']
        // Eg. for jmpoff _target, offset -> ['_target', 'offset']

        // Inject all macro instructions
        printf("INJECTING: ");
        int error = 0;
        for (int i = 0; i < macro.body.length; i++){
            Instruction currentInstruction = macro.body.Instructions[i];
            Instruction modifiedInstruction;

            // TODO: fulfill macro operands from prototype
            // TODO: what if operand arg, and global? I don't think
            // it would cause problems but should it be allowed?

            // Create new instruction in function

            // Write function to get index of needed operands from  
            // macro definition
            // Eg. in PPC _lbl, call _lbl would look up address of 
            // _lbl in the prototype (PPC _lbl), return 0

            // Use index of argument in macro prototype to look up
            // argumment in the list of macro oeprands
            // Eg. for PPC _start, Call _lbl would look up index 0
            // from the list of macro operands and get _start

            printf(" %s ",keywords[modifiedInstruction.instructionType]);
            modifiedInstruction = createModifiedInstruction(macro, currentInstruction, instruction);

            error = addInstruction(processedProgram, modifiedInstruction, 1);
            if (error != 1) return -1;
        }
        printf("\n");

    } else {
        // No preprocessor action necessary is not in macro
        if (currentMacro == -1){
            printf("No preprocessor action\n");
            return addInstruction(processedProgram, instruction, 1);
        } else { 
            // If in macro put instructions into it
            printf("YES preprocessor action\n");
            Program *macroContents = &valid_macros->macros[currentMacro].body;
            return addInstruction(macroContents, instruction, 1);
        }

    }

    // All good
    return 1;
}

// Inject file contents from include statement in its place (index)
int include_file(Program *program, MacroTable *macros, int index){
    // Get the include statement
    Instruction include = program->Instructions[index];

    if (include.operandsLength != 1 || include.operands[0].accesingMode != STRING) {
        printf(".INCLUDE did not have 1 operand of type string.\n");
        return 0;
    }

    // Get the path and open file
    char *path = include.operands[0].value;
    printf("Attempting to include \"%s\"\n", path);

    FILE *fp;
    if ((fp = fopen(path, "r")) == NULL){
        printf("Failed to include %s.\n", path);
        return 0;
    }

    // Read file contents into local program
    printf("Reading file!\n");

    Program file_contents;
    file_contents.length = 0;
    file_contents.Instructions = malloc(0);
    
    parseProgram(fp, &file_contents, macros);

    if (file_contents.length == -1) return 0;
    printf("Read file!\n");

    // Inject file contents at current location (code past .INCLUDE hasn't been parsed yet)
    for (int i = 0; i < file_contents.length; i++) {
        if (!addInstruction(program, file_contents.Instructions[i], 1)){
            printf("Problem occured Injecting included code.\n");
            return 0;
        }

    }
    

    return 1;
}

// Turn lexed token stream into a processed stream fit for generation
int preprocessProgram(Program *program, MacroTable *macros, Program *processed){
    // TODO: Take in multiple programs like gcc and process instruction for all
    // TODO: free unused instructions

    // Run every instruction in program through preprocessor and put result in processed
    for (int i = 0; i < program->length; i++) {
        printf("Processing %d %s: ", i, keywords[program->Instructions[i].instructionType]);

        char return_code = preprocessInstruction(processed, macros, program->Instructions[i]);
        if (return_code <= 0) return 0;
        else if (return_code == INCLUDE_FILE) {
            if (include_file(program, macros, i) == 0) return 0;
        }
    }
    
    printf("Processed output contains %d entries\n",processed->length);

    return 1;
}