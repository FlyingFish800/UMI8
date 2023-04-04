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

// Decodes and indirect token into an for the address it represents
int decode_indirect(char *indirect){
    // Check for first [ and skip it
    if (*indirect == '[') ++indirect;
    else return -1;
    
    // Check find last ] and make sure it exists
    int i = 0;
    while (indirect[i] != ']') if (indirect[i++] == '\0') return -1;
    
    // Decode as an immediate from there
    return decodeImmediate(indirect);
}

// Handle all variants of the LD ?, ? instruction and return the size in bytes
int handleLD(Instruction ins, LabelTable *deferred, char *code, int address){ // TODO: All handle functions need char *machinecode to add their opcodes
    printf("0x%x LD ", address);

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case REGISTER: // LD reg, ?
            char reg = *ins.operands[0].value;
            if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR LD reg: %c\n", reg); return -1;}
            unsigned char to_C = (*ins.operands[0].value == 'A') ? 0 : OP_ATOC;
            printf("LD %c ", reg);
            switch (ins.operands[1].accesingMode) {
                case IMMEDIATE: // LD reg, I    2 Bytes
                    printf("LD %c, I\n", reg);
                    code[address] = OP_LDAI + to_C; 
                    code[++address] = decodeImmediate(ins.operands[1].value);
                    return 2;
                    break;

                case ABSOLUTE: // LD reg, [imm]
                    printf("LD %c, [I] ", reg);
                    code[address] = OP_LDAMA + to_C; // LD A, MemAbs
                    int immediate_address = decode_indirect(ins.operands[1].value);
                    code[++address] = (immediate_address&0xFF);
                    code[++address] = (immediate_address&0xFF00)>>8;
                    return 3;
                    break;

                case ABSOLUTE_LABEL: // LD reg, _lbl    3 Bytes
                    // TODO: double check. I was tired doing this ;P
                    printf("LD %c, _lbl\n", reg);
                    code[address] = OP_LDAMA + to_C; 
                    address++;
                    // TODO: Segfault here because something is appended to operands[1].value? Messes with address of _char
                    addLabel(deferred, (Label) {ins.operands[1].value, address}); // TODO: Add new label
                    return 3;
                    break;
                
                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD reg, %i\n", ins.operands[0].accesingMode);
                    return -1;
                    break;
            }
            break;

        case ABSOLUTE: // LD [imm], ?   TODO: CHECK IF ZERO PAGE
            printf("LD [I] ");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD [imm], R   3 Bytes
                    char reg = *ins.operands[1].value;
                    if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR LD [i], reg: %c\n", reg); return -1;}
                    unsigned char to_C = (*ins.operands[1].value == 'A') ? 0 : OP_ATOC;

                    printf("LD [I], %c \n", reg);

                    code[address] = OP_LDMAA + to_C; 
                    int immediate_address = decode_indirect(ins.operands[0].value);
                    code[++address] = (immediate_address&0xFF);
                    code[++address] = (immediate_address&0xFF00)>>8;
                    return 3;
                    break;
                
                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD [imm], %i\n", ins.operands[0].accesingMode);
                    return -1;
                    break;
            }
            break;

        case ABSOLUTE_LABEL:
            printf("LD _lbl ");

            switch (ins.operands[1].accesingMode) {
                case REGISTER: // LD _lbl, R   3 Bytes
                    char reg = *ins.operands[1].value;
                    if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR LD _lbl, reg: %c\n", reg); return -1;}
                    unsigned char to_C = (*ins.operands[1].value == 'A') ? 0 : OP_ATOC;

                    printf("LD _lbl, %c\n", reg);
                    code[address] = OP_LDMAA + to_C;
                    addLabel(deferred, (Label) {ins.operands[0].value, ++address}); // TODO: Add new label
                    return 3;
                    break;

                default:
                    printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR LD _LBL, %i\n", ins.operands[0].accesingMode);
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

// Handle variants of the JP instruction (all fit op <imm> template including CALL,  
// and necessitate deferred generation for labels) and return size in bytes
int handleJP(Instruction ins, LabelTable *deferred, char *code, int address, char opcode, char type){
    printf("0x%x %s ", address, keywords[type]);

    // Check to see which variant is used
    switch (ins.operands[0].accesingMode) {
        case IMMEDIATE: // JP addr
            printf("%s [ADDR]\n", keywords[type]);
            code[address] = opcode; 
            int immediate_address = decodeImmediate(ins.operands[0].value);
            code[++address] = (immediate_address&0xFF);
            code[++address] = (immediate_address&0xFF00)>>8;
            return 3;
            break;

        case ABSOLUTE_LABEL: // JP label
            printf("%s _label\n", keywords[type]);
            code[address] = opcode;
            code[++address] = 'L';
            code[++address] = 'L';
            addLabel(deferred, (Label) {ins.operands[0].value, --address}); // TODO: Add new label
            //addInstructionDeferred(deferred, ins);
            return 3;
            break;
        
        default:
            printf("INVALID/UNIMPLEMENTED OPERAND TYPE FOR %s %i\n", keywords[type], ins.operands[0].accesingMode);
            return -1;
    }
    return 0;
}

// Look up address of label "name" in the label table
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
        int type = ins->instructionType;
        if (strcmp(keywords[type], "LABEL") == 0){
                // Labels dont change address, but they need their address recorded
                Label l;
                l.address = address;
                l.identifier = ins->operands[0].value;

                // Add label and handle errors if necessary
                if(addLabel(&lt, l) < 0) return -1;
                printf("Label %i found '%s' @0x%x\n", lt.length, ins->operands[0].value, l.address);
                
        } else if (strcmp(keywords[type], ".ORG") == 0){
                // ORG directly sets address
                address = decodeImmediate(ins->operands[0].value);
                if (address == -1) {
                    printf("UNABLE TO DECODE IMMEDIATE VALUE FOR ORG %s\n", ins->operands[0].value);
                    return -1;
                }
                printf("Organized to %i\n", address);
                
        } else if (strcmp(keywords[type], ".DB") == 0){
                // ORG directly sets bytes
                printf("0x%x DB: ", address);
                for (int i = 0; i < ins->operandsLength; i++){
                    if (ins->operands[i].accesingMode != IMMEDIATE) {
                        printf("BAD ACCESSING MODE FOR DB %i\n", ins->operands[i].accesingMode);
                        return -1;
                    }
                    printf("0x%x ", decodeImmediate(ins->operands[i].value) & 0xFF);
                    machineCode[address++] = decodeImmediate(ins->operands[i].value) & 0xFF;
                }
                printf("\n");
                
        } else if (strcmp(keywords[type], ".ASCII") == 0){
                // ASCII sets bytes of an ascii string
                printf("ASCII :");
                for (int i = 0; i < strlen(ins->operands[0].value); i++){
                    char c = ins->operands[0].value[i];
                    if (c != '"') {
                        printf("%c", c);
                        machineCode[address++] = c;
                    }
                }
                printf("\n");
                
        } else if (strcmp(keywords[type], ".GLOBAL") == 0){
                printf("Global entrypoint found: %s\n", ins->operands[0].value);
                // Global doesnt affect address

        }else if (strcmp(keywords[type], "LD") == 0){
                // Load can have variable length based on which variant is used. All should have 2 operands though
                if (ins->operandsLength != 2) {printf("INVALID OPERANDS LENGTH %i FOR LD\n", ins->operandsLength); return -1;}
                size = handleLD(*ins, &deferredGen, machineCode, address);
                if (size == -1) return -1;
                address += size;

        } else if (strcmp(keywords[type], "JP") == 0){
                // Load can have variable length based on which variant is used. All should have 1 operand though
                if (ins->operandsLength != 1) {printf("INVALID OPERANDS LENGTH %i FOR JP\n", ins->operandsLength); return -1;}
                size = handleJP(*ins, &deferredGen, machineCode, address, OP_JPI, type);
                if (size == -1) return -1;
                address += size;

        } else if (strcmp(keywords[type], "JZ") == 0){
                // Load can have variable length based on which variant is used. All should have 1 operand though
                if (ins->operandsLength != 1) {printf("INVALID OPERANDS LENGTH %i FOR JPZ\n", ins->operandsLength); return -1;}
                size = handleJP(*ins, &deferredGen, machineCode, address, OP_JPZI, type);
                if (size == -1) return -1;
                address += size;

        } else if (strcmp(keywords[type], "CALL") == 0){
                // Load can have variable length based on which variant is used. All should have 1 operand though
                if (ins->operandsLength != 1) {printf("INVALID OPERANDS LENGTH %i FOR CALL\n", ins->operandsLength); return -1;}
                size = handleJP(*ins, &deferredGen, machineCode, address, OP_CALLI, type);
                if (size == -1) return -1;
                address += size;

        } else if (strcmp(keywords[type], "NOP") == 0){
                // All these instructions are one byte with no args
                machineCode[address] = OP_NOP;
                address += 1;

        } else if (strcmp(keywords[type], "RET") == 0){
                // All these instructions are one byte with no args
                machineCode[address] = OP_RET;
                address += 1;

        } else if (strcmp(keywords[type], "PUSH") == 0){
                // All these instructions are one byte with no args
                if (ins->operandsLength != 1) {printf("INVALID OPERAND LENGTH %d, EXPECTED 1 FOR PUSH\n", ins->operandsLength); return -1;}
                
                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR PUSH reg: %c\n", reg); return -1;}
                unsigned char to_C = (*ins->operands[0].value == 'A') ? 0 : OP_ATOC;

                machineCode[address] = OP_PUSHR + to_C;
                address += 1;

        } else if (strcmp(keywords[type], "POP") == 0){
                // All these instructions are one byte with no args
                if (ins->operandsLength != 1) {printf("INVALID OPERAND LENGTH %d, EXPECTED 1 FOR POP\n", ins->operandsLength); return -1;}
                
                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR POP reg: %c\n", reg); return -1;}
                unsigned char to_C = (*ins->operands[0].value == 'A') ? 0 : OP_ATOC;

                machineCode[address] = OP_POPR + to_C;
                address += 1;

        } else if (strcmp(keywords[type], "ADD") == 0){
                // All these instructions are one byte with no args
                if (ins->operands[0].accesingMode != REGISTER || ins->operands[1].accesingMode != IMMEDIATE) {
                    printf("ADD MUST BE OF FORM ADD <reg>, <imm>");
                    return -1;
                }

                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR ADD <reg>, <imm>: %c\n", reg); return -1;}
                unsigned char to_C = (reg == 'A') ? 0 : OP_ATOC;

                machineCode[address++] = OP_ADDIA + to_C;
                machineCode[address++] = decodeImmediate(ins->operands[1].value) & 0xFF;

        } else if (strcmp(keywords[type], "ADDC") == 0){
                // All these instructions are one byte with no args
                if (ins->operands[0].accesingMode != REGISTER || ins->operands[1].accesingMode != IMMEDIATE) {
                    printf("ADD MUST BE OF FORM ADDC <reg>, <imm>");
                    return -1;
                }

                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR ADDC <reg>, <imm>: %c\n", reg); return -1;}
                unsigned char to_C = (reg == 'A') ? 0 : OP_ATOC;

                machineCode[address++] = OP_ADDCIA + to_C;
                machineCode[address++] = decodeImmediate(ins->operands[1].value) & 0xFF;

        } else if (strcmp(keywords[type], "SUB") == 0){
                // All these instructions are one byte with no args
                if (ins->operands[0].accesingMode != REGISTER || ins->operands[1].accesingMode != IMMEDIATE) {
                    printf("ADD MUST BE OF FORM SUB <reg>, <imm>");
                    return -1;
                }

                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR SUB <reg>, <imm>: %c\n", reg); return -1;}
                unsigned char to_C = (reg == 'A') ? 0 : OP_ATOC;

                machineCode[address++] = OP_SUBIA + to_C;
                machineCode[address++] = decodeImmediate(ins->operands[1].value) & 0xFF;

        } else if (strcmp(keywords[type], "CMP") == 0){
                // All these instructions are one byte with no args
                if (ins->operands[0].accesingMode != REGISTER || ins->operands[1].accesingMode != IMMEDIATE) {
                    printf("ADD MUST BE OF FORM CMP <reg>, <imm>");
                    return -1;
                }

                char reg = *ins->operands[0].value;
                if (reg != 'C' && reg != 'A') {printf("INVALID REGISTER FOR SUB <reg>, <imm>: %c\n", reg); return -1;}
                unsigned char to_C = (reg == 'A') ? 0 : OP_ATOC;

                machineCode[address++] = OP_CMPIA + to_C;
                machineCode[address++] = decodeImmediate(ins->operands[1].value) & 0xFF;

        } else {
                printf("UNKNOWN/UNIMPLEMENTED INSTRUCTION %s WITH %d OPERANDS (", keywords[ins->instructionType], ins->operandsLength);
                for (int i = 0; i < ins->operandsLength; i++) {
                    printf(" %s%c", ins->operands[i].value, (i == ins->operandsLength-1) ? ' ' : ',');
                }
                
                printf(") IN POPULATE generateCode\n");
                return -1;
        }
        if (address > max_addr) max_addr = address;
    }

    // LABEL TABLE NOW POPULATED. RESOLVE INSTRUCTIONS THAT RELY ON LABELS

    printf("\nPRINTING LABEL TABLE:\n");
    for (int i = 0; i < lt.length; i++) {
        printf("%s 0x%x\n", lt.labels[i].identifier, lt.labels[i].address);
    }


    // Works, doesnt print NOP
    printf("\nMACHINE CODE:\n");
    for (int i = 0; i < PROGRAM_SIZE; i++)
    {
        if(machineCode[i] != 0) printf("%c",machineCode[i]);
    }

    printf("\n");

    
    for (int i = 0; i < deferredGen.length; i++){
        Label current_label = deferredGen.labels[i];
        printf("DEF: Inject addr of %s @ address 0x%x\n", current_label.identifier, current_label.address);
        // TODO: Find address of labels in deffered label and put found label address at deffered label and deffered+1
        int label_address = -1;
        if ((label_address = get_label_address(&lt, current_label.identifier)) >= 0){
            machineCode[current_label.address] = label_address&0xFF;
            machineCode[current_label.address+1] = (label_address&0xFF00) >> 8;
        } else {
            printf("COULDN'T FIND LABEL %s\n", current_label.identifier);
            return -1;

        }
    }
    
    FILE *fp = fopen("./a.bin", "w");

    printf("MAX ADDRESS: 0x%x\n", max_addr);

    for (int i = 0; i < max_addr; i++){
        fputc(machineCode[i], fp);
    }

    fclose(fp);

    return 0;
}