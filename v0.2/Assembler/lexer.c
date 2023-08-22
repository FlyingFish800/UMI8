/*  Alexander Symons | Nov 6 2022 | Assembler | lexer.c
 *  Program containing all Assembly lexing functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "lexer.h"
#include "preprocessor.h"

// Returns true if character is whitespace
int isWhiteSpace(char c){
    return c == ' ' || c == '\t' || c == ',';
}

// Returns true if character is comment or end of line
int isComment(char c){
    return c == ';' || c == '\n';
}

// Skip whtie space to the next important section
void skipWhiteSpace(FILE *fp) {
    char c;
    
    // Keep getting chars until you get one that isnt whitespace
    do {
        c = fgetc(fp);
    } while (isWhiteSpace(c));

    // Then put the non-whitespace char back
    ungetc(c, fp);
}

// Skip everything until next newline or eof
void skipComments(FILE *fp) {
    char c;
    
    // Keep getting chars until you get one that isnt whitespace
    do {
        c = fgetc(fp);
        if (c == EOF) return;
    } while (c != '\n');

    // Then put the non-whitespace char back
    ungetc(c, fp);
}

// Reusable code for parsing, gets fp to next relevent block of code. Doesnt sknip \n or EOF. Returns true is EOF
int getNextBlock(FILE *fp){
    // Skip precceding whitespace
    skipWhiteSpace(fp);

    // Check if line is just comment or EOF
    char c = fgetc(fp);
    if (c == EOF) return 1;
    if (c == ';') {
        skipComments(fp);
        c = fgetc(fp); // \n
        if (c == EOF) return 1;
        ungetc(c,fp); // Put \n bacl
        return 0;
    } else {
        // Not comment, put char back
        ungetc(c, fp);
        return 0;
    }
}

// Read an instruction, and add it to the instructions pointer. Return 1 if there is more to parse, 0 for EOF
int parseInstruction(FILE *fp, Program *program, MacroTable *valid_macros){
    /* Handles lines that are just \n or comments.
     * 1: Skip white space and check if comment
     * 2: Read identifier
     * 3: Skip white space
     * 4: Read operands
     * 5: Repeat 3-4 as necessary (OR dont do them at all)
     * 6: Handle comments and newline(s), unget EOFs
     */

    // Track if in a macro definition
    static char in_macro = 0;

    // Get next block. If returns true for EOF, no more to parse
    if (getNextBlock(fp)) return PARSE_ERROR_NONE;
    char c = fgetc(fp);
    if (c == '\n') return PARSE_CONTINUE;

    // Read Identifier, read char by char
    // TODO: Memsafe?
    int len = 0;
    char *id = malloc(0);
    do {
        len += 1;
        id = realloc(id,len*sizeof(char));
        if (id == NULL) {printf("ERROR, UNABLE TO REALLOC IN ID PARSING"); return PARSE_ERROR_REALLOC;}
        id[len-1] = c;
        c = fgetc(fp);
    } while (!isWhiteSpace(c) && c != ';' && c != '\n' && c != EOF);
    ungetc(c, fp);

    // When building strings yourself, don't forget to null terminate them!
    // TODO: err check
    id = realloc(id,(len+1)*sizeof(char));
    id[len] = '\0';

    // Prepare instruction for operands
    Instruction instruction;

    instruction.operandsLength = 0;
    instruction.operands = malloc(0);
    if (instruction.operands == NULL) {
        perror("FAILED INITIAL ALLOCATION OF OPERANDS"); 
        program->length = -1;
        return PARSE_ERROR_MALLOC;
    }

    // In case it is a macro
    Macro new_macro;

    // Determine enum type
    int type;
    if(id[0] == '_') {
        id[len-1] = '\0';
        instruction.instructionType = keyword_to_type("LABEL");


        // Push label identifier as operand
        if(id[len-1] == ':') id[len-1] = '\0'; // TODO: Redundant?
        Operand op;
        op.accesingMode = NONE;
        op.value = id;
        if(!addOperand(&instruction, op)) {
            printf("FAILED TO ADD OPERAND FOR LABEL %s\n", id);
            program->length = -1;
            return PARSE_ERROR_INVALID_OPERAND;
        }

    } else if ((type = keyword_to_type(id)) >= 0) {
        // Handle all valid keywords
        
        if (type == keyword_to_type(".MACRO")){
            // Remember that you are inside macro definintion
            in_macro = 1;

            // Set up macro struct as well to register it as valid
            new_macro.identifier = id;
            new_macro.operandsLength = 0;
            new_macro.operands = malloc(0);
            if (new_macro.operands == NULL){
                printf("UNABLE TO ALLOCATE MEMORY FOR MACRO %s OPERANDS.\n", id);
                program->length = -1;
                return PARSE_ERROR_MALLOC;
            }
        }

        // End strops weird rule exceptions
        if (type == keyword_to_type(".END")){
            printf("MACRO DEF DONE. ");
            in_macro = 0;
        }

        instruction.instructionType = type;
    } else {
        char isValidMacro = 0;
        for (int i = 0; i < valid_macros->length; i++){
            printf("Try: [%s] ", valid_macros->macros[i].identifier);
            if (strcmp(id, valid_macros->macros[i].identifier) == 0) isValidMacro = 1;
        }
        
        if (!isValidMacro){
            printf("%s IS NOT A VALID MACRO.\n", id);
            printf("UNIMPLEMENTED/INVALID INSTRUCTION <%s> IN PARSING\n",id);
            program->length = -1;
            return PARSE_ERROR_INVALID_INSTRUCTION;
        } else {
            // Macro valid
            printf("%s IS A VALID MACRO.\n", id);
            instruction.instructionType = keyword_to_type("INVOKE_MACRO");
            
            Operand name;
            name.accesingMode = MACRO_ID;
            name.value = id;
            addOperand(&instruction, name);
        }
    }

    printf("INS <%s%s> ", id, (in_macro) ? " <M>" : "");

    // -----HANDLE OPERANDS-----
    // Parse next block
    Operand operand;
    while (1){ 
        // Get next block. If returns true for EOF, no more to parse. Pushes instruction to list    
        if (getNextBlock(fp)) {
            if (type == keyword_to_type(".MACRO")) registerMacro(valid_macros, new_macro, 0);
            return addInstruction(program, instruction, 0);
        }   
        c = fgetc(fp);
        if (c == '\n') {
            if (type == keyword_to_type(".MACRO")) registerMacro(valid_macros, new_macro, 1);
            return addInstruction(program, instruction, 1);
        }

        // Parse Operand identifier
        int opLen = 0;
        char *opId = malloc(0);
        int string = (c == '"');
        do {
            opLen+=1;
            opId = realloc(opId,opLen*sizeof(char));
            if (opId == NULL) {printf("ERROR, UNABLE TO REALLOC IN ID PARSING"); return PARSE_ERROR_REALLOC;}
            opId[opLen-1] = c;
            c = fgetc(fp);
            if (c == '"') string = 0;
        } while ((string || !isWhiteSpace(c)) && c != ';' && c != '\n' && c != EOF);
        ungetc(c, fp);

        // When building strings yourself, dont forget to null terminate them!
        opId = realloc(opId,(opLen+1)*sizeof(char)); // TODO: ERR check
        opId[opLen] = '\0';

        // Initialize operand
        operand.value = opId;
        operand.accesingMode = 0;

        // Set accessing mode
        if(operand.value[0] == '_') {
            printf("ABS_LABEL");
            operand.accesingMode = ABSOLUTE_LABEL;
        } else if((operand.value[0] == 'A' || opId[0] == 'C') && (operand.value[1] == '\0')) {
            printf("REG");
            operand.accesingMode = REGISTER;
        } else if(operand.value[0] == '#') {
            printf("IMM");
            operand.accesingMode = IMMEDIATE;
        } else if(operand.value[0] == '[') {
            if(operand.value[1] == '[') { // Double memory lookup [[#0xCAFE]]
                printf("REL");
                operand.accesingMode = RELATIVE;
            }  else if(operand.value[1] == '_') { // Memory lookup of label [_pointer]
                printf("REL_LABEL");
                operand.accesingMode = RELATIVE_LABEL;
            } else { // Single lookup of immediate [#0xCAFE]
                printf("ABS");
                operand.accesingMode = ABSOLUTE;
            }
        } else if(operand.value[0] == '"') {
            printf("STR");
            operand.accesingMode = STRING;
        } else if(operand.value[0] == '<') {
            // Make sure this is only used for macros
            if (instruction.instructionType != keyword_to_type(".MACRO")){
                printf("MACRO PROTOTYPE %s GIVEN IN NON MACRO DEFINITION (%s)\n", operand.value, opId);
                program->length = -1;
                return PARSE_ERROR_INVALID_MACRO;
            }

            // Expected arguments given to macro in form <protype>
            printf("MACRO ARG");
            operand.accesingMode = MACRO_ARG;

            // Trim leading '<' and trailing '>'
            char *start = ++operand.value;
            while (*(start++) != '>');
            *(--start) = '\0';

            // Push operand to macro too to check macro invocations
            if(!registerMacroOperand(&new_macro, operand)) {
                printf("FAILED TO ADD OPERAND %s FOR MACRO %s\n", operand.value, id);
                program->length = -1;
                return PARSE_ERROR_ADD_OEPRAND;
            } 

        } else if (instruction.instructionType == keyword_to_type(".MACRO") && instruction.operandsLength == 0){
            // First operand of a macro is the identifier
            //printf("MACRO ID [%s] ", operand.value);
            
            new_macro.identifier = operand.value;
            operand.accesingMode = MACRO_ID;
        } else {
            if (!in_macro) {
                printf("UNIMPLEMENTED/INVALID OPERAND <%s> IN PARSING\n",opId);
                program->length = -1;
                return PARSE_ERROR_INVALID_OPERAND;
            } else {
                // Failed every other check. Probably macro prototype
            }
        }

        printf(":%s ",operand.value);

        // Push operand to instruction. If it returns a 1, quit
        if(!addOperand(&instruction, operand)) {
            printf("FAILED TO ADD OPERAND %s FOR INSTRUCTION %s\n", operand.value, id);
            program->length = -1;
            return PARSE_ERROR_ADD_OEPRAND;
        } 
    }
}

// Parse file given to function
void parseProgram(FILE *fp, Program *program){
    // Create program that get freed for the intermediate lexing
    Program unprocessed;
    unprocessed.length = 0;
    unprocessed.Instructions = malloc(0);
    if (unprocessed.Instructions == NULL) printf("FAILED INITIAL MALLOC FOR UNPROCESSED INSTRUCTIONS");

    // Initialize the program to be returned containing processed instructions
    program->length = 0;
    program->Instructions = malloc(0);
    if (program->Instructions == NULL) printf("FAILED INITIAL MALLOC FOR INSTRUCTIONS");

    // Track identifiers for valid macros that have been defined.
    MacroTable valid_macros;
    valid_macros.length = 0;
    valid_macros.macros = malloc(0);

    // Get file pointer to first valid code block (or comment, which is handled in same code)
    skipWhiteSpace(fp);

    // ParseInstruction for every line of file, handles comments and '\n'. breaks look when EOF encoutnered
    int error;
    while (error = parseInstruction(fp, &unprocessed, &valid_macros)) printf("\n");
    // TODO: Increment counter. Line with error is in counter

    // Print macros
    printf("\nMacros found: %d\n", valid_macros.length);
    for (int i = 0; i < valid_macros.length; i++) printf("MACRO %d: %s", i+1, valid_macros.macros[i].identifier);

    printf("\n");

    // Preprocess Program
    printf("\n----BEGINING-PREPROCESSING----\n");
    printf("Preprocessing returned: %d\n", preprocessProgram(&unprocessed, valid_macros, program));
    printf("%d\n", program->length);

    // Free unprocessed instructions array pointer
    // DON'T free anything else, those instructions are put directly into new program
    free(unprocessed.Instructions);

}