#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

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

// Basic driver for testing
int main(){
   printf("The number(unsigned long integer) is %i\n", toInt("0b10"));
   printf("The number(unsigned long integer) is %i\n", toInt("0x10"));
   printf("The number(unsigned long integer) is %i\n", toInt("0o10"));
   printf("The number(unsigned long integer) is %i\n", toInt("0010"));

   return 0;
}

// Generate output file from program generated by parser
int generateCode(Program *program, FILE *outFile){
    for (int i = 0; i < program->length; i++){
        switch (program->Instructions[i].instructionType) {
            case GLOBAL:
                printf("Global entrypoint found: %s\n", program->Instructions[i].operands[0].value);
                break;
            
            default:
                break;
        }
    }
    return 0;
}

