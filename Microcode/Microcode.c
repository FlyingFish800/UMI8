#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SRC strl lines
#define II  0b00000000;
#define MIH 0b00100000;
#define MIL 0b01000000;
#define PIH 0b01100000;
#define PIL 0b10000000;
#define AI  0b10100000;
#define CR  0b11000000;
#define WR  0b11100000;

// DST ctrl lines
#define NA  0b00000000;
#define POL 0b00001000;
#define POH 0b00010000;
#define RD  0b00011000;

// ALU/Other ctrl lines
#define CE  0b00000100
#define SUB 0b00000010
#define LD  0b00000001

// Const for file reading
#define LINESIZE  256

// How many microsteps
#define MICROSTEPS 16

// Microcode array for binary
char microcode[8192] = {0};

int main (){
    FILE* fpointer = fopen("microcode.mc","r");
    char buffer[LINESIZE];

    if (fpointer == NULL) {printf("Error reading File"); return 1;}

    while (fgets(buffer, LINESIZE, fpointer) != NULL){
        processLine(buffer);
    } // end while

    fclose(fpointer);

    printf("\n");

    return 0;
} // end main

// turn a line into microcode
void processLine(char line[]){
    // String pointer for chunks of data
    char *sp;

    // Buffer for trimmed control word string
    char word[20];

    // Get up to comma delimiter
    sp = strtok(line, ",");

    // Get index of ctrl word, and opcode
    int index = 0;
    int opcode;

    // For every control word
    while (sp != NULL) {
        // Binary control word
        char ctrlBin = 0x0;

        // end of what should be copied
        int end = 20;

        // trim if there is a comment. Clear so there isnt any trash
        for (size_t i = 0; i < 20; i++){
            if(!isspace(*sp) && sp[i] == ';') end = i;
            if(isspace(*sp)) sp++;
            word[i] = 0;
        } // end for
        
        // copy up to ;
        strncpy(word,sp,end);

        printf("%s\n",word);

        // Set to 0 by default
        opcode = 0;

        // String pointer for individual ctrl lines
        char *ctrlLine;

        // either handle ctrl lines or set location
        if(index == 0){
            // convert opcode
            opcode = (int) strtol(word,NULL,16);
            // TODO: Flags
        } else {

        } // end if

        // Get next portion
        sp = strtok(NULL, ",");
        index++;
    } // end while
    
} // end processline