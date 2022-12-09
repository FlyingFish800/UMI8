#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "structs.h"

// Size of 16bit address space
#define PROGRAM_SIZE 0x10000

// Convert string to int regarudless of base
int toInt(char *string){
    // Get bbase of nput
    int base = 10;
    
    if (string[1] == 'x') base = 16;
    if (string[1] == 'o') base = 8;
    if (string[1] == 'b') base = 2;

    // Cut of 0x, 0b, 0o or whatever if necesarry
    int strStart = 0;
    if (base != 10) strStart = 2;
    char *trimmedStr = malloc(0);

    while(string[strStart] != '\0'){
        if (base == 10) {
            // Base 10 is clean copy
            trimmedStr = realloc(trimmedStr, strStart+1); 
            trimmedStr[strStart] = string[strStart];
        } else {
            // Other bases need first two chars trimmed
            trimmedStr = realloc(trimmedStr, strStart-1);
            trimmedStr[strStart-2] = string[strStart];
        }
        strStart++;
    }

    // return strtol
    return strtol(trimmedStr,NULL,base);
}

// Push a label to the label table
int addLabel (LabelTable *lt, Label l){
    lt->length += 1;
    lt->labels = realloc(lt->labels, sizeof(Label)*lt->length);
    if (lt->labels == NULL) {perror("COULDNT REALLOC LABELS"); return 0;}
    lt->labels[lt->length-1] = l;
    return 0;
}

// Decodes immediate value and returns its value
int decodeImmediate(char *immediate){
    if (*immediate == '#') return toInt(++immediate);
    printf("VALUE IS NOT AN IMEMDIATE: %s\n", immediate);
    return -1;
}

// Handle all variants of the LD ?, ? instruction and return the size in bytes
int handleLD(Instruction ins){
    printf("LD");

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case REGISTER: // LD reg, ?
            printf("LD R");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD reg, reg   1 Byte
                    printf("LD R, R");
                    printf("%s %s", ins.operands[0].value, ins.operands[1].value);
                    return 1;
                    break;

                case IMMEDIATE: // LD reg, I    2 Bytes
                    printf("LD R, I");
                    return 2;
                    break;
                
                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD reg, %i\n", ins.operands[0].accesingMode);
                    return -1;
                    break;
            }
            break;

        case INDIRECT: // LD [imm], ?   TODO: CHECK IF ZERO PAGE
            printf("LD [I]");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD [imm], R   3 Bytes
                    printf("LD [I], R");
                    return 3;
                    break;

                case IMMEDIATE: // LD [imm], I  4 Bytes
                    printf("LD [I], I");
                    return 4;
                    break;
                
                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD [imm], %i\n", ins.operands[0].accesingMode);
                    return -1;
                    break;
            }
            break;

        default:
            printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD %i\n", ins.operands[0].accesingMode);
            return -1;
    }
    return 0;
}

// Generate output file from program generated by parser
int generateCode(Program *program, FILE *outFile){
    // Initialize table of labels and machine code
    LabelTable lt;
    lt.length = 0;
    lt.labels = malloc(0);
    char *machineCode = malloc(PROGRAM_SIZE);

    // One pass approach where code is generated, and labels are resolved at the end
    
    // Track address
    int address = 0;

    // Find address of each instruction
    for (int i = 0; i < program->length; i++){
        Instruction *ins = &program->Instructions[i];

        // Switch on instruction type
        switch (ins->instructionType) {
            case LABEL:
                // Labels dont change address, but they need their address recorded
                Label l;
                l.address = address;
                l.identifier = ins->operands[0].value;

                // Add label and handle errors if necessary
                if(addLabel(&lt, l) < 0) return -1;
                printf("Label %i found '%s' @0x%x\n", lt.length, ins->operands[0].value, l.address);
                break;

            case ORG:
                // ORG directly sets address
                address = decodeImmediate(ins->operands[0].value);
                if (address == -1) return -1;
                printf("Organized to %i\n", address);
                break;

            case GLOBAL:
                printf("Global entrypoint found: %s\n", ins->operands[0].value);
                // Global doesnt affect address
                break;

            case LD:
                // Load can have variable length based on which variant is used. All should have 2 operands though
                if (ins->operandsLength != 2) {printf("INVALID OPERANDS LENGTH %i FOR LD\n", ins->operandsLength); return -1;}
                int size = handleLD(*ins);
                if (size == -1) return -1;
                address += size;
                break;

            case NOP:
                // All these instructions are one byte with no args
                machineCode[address] = 0x0;
                address += 1;
                break;

            case RET:
                // All these instructions are one byte with no args
                machineCode[address] = 0x22;
                address += 1;
                break;

            default:
                printf("UNKNOWN INSTRUCTION %i IN POPULATE LABEL TABLE\n", ins->instructionType);
                return -1;
                break;
        }
    }

    // LABEL TABLE NOW POPULATED. RESOLVE INSTRUCTIONS THAT RELY ON LABELS

    printf("PRINTING LABEL TABLE:\n");
    for (int i = 0; i < lt.length; i++) {
        printf("%s 0x%x\n", lt.labels[i].identifier, lt.labels[i].address);
    }


    // Works, doesnt print NOP
    printf("MACHINE CODE:\n");
    for (int i = 0; i < PROGRAM_SIZE; i++)
    {
        if(machineCode[i] != 0) printf("%c",machineCode[i]);
    }

    printf("\n");
    

    return 0;
}