#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "structs.h"
#include "opcodes.h"

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
int handleLD(Instruction ins, LabelTable *deferred, char *code, int address){ // TODO: All handle functions need char *machinecode to add their opcodes
    printf("LD ");

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case REGISTER: // LD reg, ?
            printf("LD R ");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD reg, reg   1 Byte
                    printf("LD R, R\n");
                    printf("%s %s", ins.operands[0].value, ins.operands[1].value);
                    return 1;
                    break;

                case IMMEDIATE: // LD reg, I    2 Bytes
                    if (*ins.operands[0].value == 'A'){
                        printf("LD A, I\n");
                        code[address] = OP_LDAI; 
                        code[++address] = decodeImmediate(ins.operands[1].value); 
                    } else if (*ins.operands[0].value == 'B'){
                        printf("LD B, I\n");
                        code[address] = OP_LDBI; 
                        code[++address] = decodeImmediate(ins.operands[1].value); 
                    } else {
                        printf("INVALID/UNIMPLEMENTED REGISTER FOR LD reg, %s\n", ins.operands[0].value);
                    }
                    return 2;
                    break;

                case INDIRECT_LABEL: // LD reg, I    2 Bytes
                    printf("LD R, L\n");
                    address++;
                    addLabel(deferred, (Label) {ins.operands[0].value, address}); // TODO: Add new label
                    return 2;
                    break;
                
                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD reg, %i\n", ins.operands[0].accesingMode);
                    return -1;
                    break;
            }
            break;

        case INDIRECT: // LD [imm], ?   TODO: CHECK IF ZERO PAGE
            printf("LD [I] ");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD [imm], R   3 Bytes
                    printf("LD [I], R\n");
                    return 3;
                    break;

                case IMMEDIATE: // LD [imm], I  4 Bytes
                    printf("LD [I], I\n");
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

// Handle variants of the JZ function and return size in bytes
int handleJZ(Instruction ins, LabelTable *deferred, char *code, int address){
    printf("JZ ");

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case INDIRECT: // JZ [addr]
            printf("JZ [ADDR]\n");
            return 3;
            break;

        case INDIRECT_LABEL: // JZ label
            printf("JZ _label\n");
            code[address] = OP_JZI;
            code[++address] = 'L';
            code[++address] = 'L';
            addLabel(deferred, (Label) {ins.operands[0].value, --address}); // TODO: Add new label
            return 3;
            break;
        
        default:
            printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR JZ %i\n", ins.operands[0].accesingMode);
            return -1;
    }
    return 0;
}

// Handle variants of the JP function and return size in bytes
int handleJP(Instruction ins, LabelTable *deferred, char *code, int address){
    printf("JP ");

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case INDIRECT: // JZ [addr]
            printf("JP [ADDR]\n");
            return 3;
            break;

        case INDIRECT_LABEL: // JZ label
            printf("JP _label\n");
            code[address] = OP_JPI;
            code[++address] = 'L';
            code[++address] = 'L';
            addLabel(deferred, (Label) {ins.operands[0].value, --address}); // TODO: Add new label
            //addInstructionDeferred(deferred, ins);
            return 3;
            break;
        
        default:
            printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR JP %i\n", ins.operands[0].accesingMode);
            return -1;
    }
    return 0;
}

int get_label_address(LabelTable *lt, char *name){
    for (int i = 0; i < lt->length; i++){
        if (strcmp(lt->labels[i].identifier, name) == 0) return lt->labels[i].address;
    }
    return -1;
}

// Generate output file from program generated by parser
int generateCode(Program *program, FILE *outFile){
    // One pass approach where code is generated, and labels are resolved at the end

    // Initialize table of labels and machine code
    LabelTable lt;
    lt.length = 0;
    lt.labels = malloc(0);
    char *machineCode = malloc(PROGRAM_SIZE);

    // Second label table for instructions that cant be generated until labels resolves
    LabelTable deferredGen;
    deferredGen.length = 0;
    deferredGen.labels = malloc(0);

    // Track address and size of instruction if necessary
    int address = 0;
    int size = -1;
    int max_addr = 0;

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
                size = handleLD(*ins, &deferredGen, machineCode, address);
                if (size == -1) return -1;
                address += size;
                break;

            case JZ:
                // Load can have variable length based on which variant is used. All should have 1 operand though
                if (ins->operandsLength != 1) {printf("INVALID OPERANDS LENGTH %i FOR JZ\n", ins->operandsLength); return -1;}
                size = handleJZ(*ins, &deferredGen, machineCode, address);
                if (size == -1) return -1;
                address += size;
                break;

            case JP:
                // Load can have variable length based on which variant is used. All should have 1 operand though
                if (ins->operandsLength != 1) {printf("INVALID OPERANDS LENGTH %i FOR JP\n", ins->operandsLength); return -1;}
                size = handleJP(*ins, &deferredGen, machineCode, address);
                if (size == -1) return -1;
                address += size;
                break;

            case NOP:
                // All these instructions are one byte with no args
                machineCode[address] = OP_NOP;
                address += 1;
                break;

            case ADD:
                // All these instructions are one byte with no args
                machineCode[address] = OP_ADD;
                address += 1;
                break;

            case SUB:
                // All these instructions are one byte with no args
                machineCode[address] = OP_SUB;
                address += 1;
                break;

            case RET:
                // All these instructions are one byte with no args
                machineCode[address] = OP_RET;
                address += 1;
                break;

            default:
                printf("UNKNOWN INSTRUCTION %i IN POPULATE LABEL TABLE\n", ins->instructionType);
                return -1;
                break;
        }
        if (address > max_addr) max_addr = address;
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

    
    for (int i = 0; i < deferredGen.length; i++){
        Label current_label = deferredGen.labels[i];
        printf("DEF: Inject addr of %s @ address %d\n", current_label.identifier, current_label.address);
        // TODO: Find address of labels in deffered label and put found label address at deffered label and deffered+1
        int label_address = -1;
        if ((label_address = get_label_address(&lt, current_label.identifier)) >= 0){
            machineCode[current_label.address] = (label_address&0xFF00) >> 8;
            machineCode[current_label.address+1] = label_address&0xFF;
        } else {
            printf("COULDN'T FIND LABEL %s\n", current_label.identifier);
            return -1;

        }
    }
    
    FILE *fp = fopen("./a.bin", "w");

    printf("MA:%d\n", max_addr);

    for (int i = 0; i < max_addr; i++){
        fputc(machineCode[i], fp);
    }

    fclose(fp);

    return 0;
}