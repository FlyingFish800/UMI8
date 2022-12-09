// Contains implementation of UMI 8 CPU
#include "CPU.h"

// Reset all internal registers and state of CPU.
// DOESN'T RESET MICROCODE
void reset(CPU* cpu){
    cpu->A = 0;
    cpu->B = 0;
    cpu->MARHI = 0;
    cpu->MARLO = 0;
    cpu->PCHI = 0;
    cpu->PCLO = 0;
    cpu->IR = 0;
    cpu->UStep = 0;
    cpu->Flags = 0;
    cpu->BUS = 0;
    memset(cpu->RAM,0,RAM_SIZE);
} // end reset

// Emulate one clock cycle of the cpu
void clock(CPU* cpu){
    // Figure out ctrl byte (Instruction register is &|!OOOOO where first 3 are loadA modifiers, rest are opcode)
    // Ucode indexing is formatted IIIIIFFFSSSSS where S = UStep, I = instruction, F = flags (ZNC)
    unsigned short MCIndex = ((cpu->IR & 0b11111) << 8) | ((cpu->Flags & 0b111) << 5) | (cpu->UStep & 0b11111);
    byte CTRLWord = cpu->Microcode[MCIndex];

    // Print ctrl info for debugging
    #ifdef DEBUG
    printf("UCode Index: %d   Flags: %x   Ctrl Word: 0x%x ", MCIndex, cpu->Flags, CTRLWord);
    dbgCtrlLine(CTRLWord);
    #endif

    // Output to bus
    // Switch on SRC bits for ctrl word. Push appropriate value to bus
    switch (CTRLWord & SRC) { // TODO: EO 
        case MOL:
            cpu->BUS = cpu->MARLO;
            break;

        case MOH:
            cpu->BUS = cpu->MARHI;
            break;

        case POL:
            cpu->BUS = cpu->PCLO;
            break;

        case POH:
            cpu->BUS = cpu->PCHI;
            break;

        case EO:
            byte flags = 0;
            // Add 1 if inc, also determine carry
            if((CTRLWord & ALU) == INC) {cpu->BUS = cpu->A + cpu->B + 1;if((cpu->A+cpu->B+1)>255)flags|=CARRY;}
            else {cpu->BUS = cpu->A + cpu->B;if((cpu->A+cpu->B)>255)flags|=CARRY;}
            if(cpu->BUS == 0) flags |= ZERO;   // Zero
            if((cpu->BUS&0b10000000) == 1) flags |= NEGATIVE;   // Negative
            break;

        case AO:
            cpu->BUS = cpu->A;
            break;
            
        case RD:
            // bool to stop reading ram after device read
            byte inhibitRAM = 0;

            // Look for device at current address
            for (word i = 0; i < cpu->deviceCount; i++){
                // if device registered at this address
                if(cpu->devices[i].address == ((cpu->MARHI << 8) | cpu->MARLO)){
                    if(cpu->devices[i].readDevice == NULL) continue; // Read mem as normal if no handler
                    else{   // otherwise read device
                        #ifdef DEBUG
                        printf("Device @[%d] read (%d)\n", cpu->devices[i].address, cpu->devices[i].readDevice());
                        #endif
                        inhibitRAM = 1;
                        cpu->BUS = cpu->devices[i].readDevice();
                        break;
                    } // end if
                } // end if
            } // end for;
            if (!inhibitRAM) cpu->BUS = cpu->RAM[(cpu->MARHI << 8) | cpu->MARLO];
            break;

        case CR:
            cpu->UStep = 0;
            return; // Would have to be implemented as not allowing bus reading, or a specific check for it?
            break;
        
        default:
            printf("ERROR: Unimplemented/Invalid value 0x%x as bus source.\n", CTRLWord&SRC);
            break;
    } // end switch

    // Input from bus
    // Switch on DST bits for ctrl word. Push appropriate value to bus
    switch (CTRLWord & DST) {
        case II:
            cpu->IR = cpu->BUS;
            break;

        case MIH:
            cpu->MARHI = cpu->BUS;
            break;
            
        case MIL:
            cpu->MARLO = cpu->BUS;
            break;
            
        case PIL:
            cpu->PCLO = cpu->BUS;
            break;
            
        case PIH:
            cpu->PCHI = cpu->BUS;
            break;
            
        case AI: // TODO: Probably cant do that in hardware
            cpu->A = cpu->BUS;
            #ifdef DEBUG
            printf("A: 0x%x BEFORE !%x |%x &%x\n", cpu->A, ((cpu->IR&LDNOT) == LDNOT), ((cpu->IR&LDOR) == LDOR), ((cpu->IR&LDAND) == LDAND));
            #endif
            if ((cpu->IR&LDNOT) == LDNOT) cpu->A = ~cpu->A;
            if ((cpu->IR&LDOR) == LDOR) cpu->A = cpu->A | cpu->B;
            if ((cpu->IR&LDAND) == LDAND) cpu->A = cpu->A & cpu->B;
            #ifdef DEBUG
            printf("A: 0x%x AFTER\n", cpu->A);
            #endif
            break;
            
        case BI:
            cpu->B = cpu->BUS;
            break;
            
        case WR:
            // bool to stop writing ram after device write
            byte inhibitRAM = 0;

            // Look for device at current address
            for (word i = 0; i < cpu->deviceCount; i++){
                // if device registered at this address
                if(cpu->devices[i].address == ((cpu->MARHI << 8) | cpu->MARLO)){
                    if(cpu->devices[i].writeDevice == NULL) continue; // Write mem as normal if no handler
                    else{   // otherwise Write device
                        #ifdef DEBUG
                        printf("Device @[%d] write (%d)\n", cpu->devices[i].address, cpu->BUS);
                        #endif
                        inhibitRAM = 1;
                        cpu->devices[i].writeDevice(cpu->BUS);
                        break;
                    } // end if
                } // end if
            } // end for;

            if(!inhibitRAM) cpu->RAM[(cpu->MARHI << 8) | cpu->MARLO] = cpu->BUS;
            break;
        
        default:
            printf("ERROR: Unimplemented/Invalid value 0x%x as bus destination.\n", CTRLWord&DST);
            break;
    } // end switch


    // Handle CE and Reset A as part of ALU operation
    if ((CTRLWord & ALU) == CE) {
        if (cpu->PCLO < 255) { // Increment least significant
            cpu->PCLO++;
        } else { // Ripple carry
            cpu->PCHI++;
            cpu->PCLO = 0;
        } // end if
    } // end if

    if ((CTRLWord & ALU) == RSA) {
        cpu->A = 0;
    } // end if
    

    // Handle microstep
    if (cpu->UStep >= 31) {
        cpu->UStep = 0;
    } else {
        cpu->UStep++;    
    } // end if
    
} // end clock

// Load file at path into the memory
void loadRam(CPU* cpu, char* path){ // TODO: implementation could be dangerous?
    memset(cpu->RAM,0,RAM_SIZE);
    FILE* file = fopen(path, "r");
    fread(cpu->RAM, 1, RAM_SIZE, file);
} // end loadRam

// Load file at path into the microcode
void loadUCode(CPU* cpu, char* path){
    memset(cpu->Microcode,0,MCODE_SIZE);
    FILE* file = fopen(path, "r");
    fread(cpu->Microcode, 1, MCODE_SIZE, file);
} // end loadRam

// Print status of cpu internal registers
void coreDump(CPU* cpu){
    printf("IR: 0x%x  Flags: %d  Step: %d\n", cpu->IR, cpu->Flags, cpu->UStep);
    printf("Bus value: 0x%x\n", cpu->BUS);
    printf("A: 0x%x  B: 0x%x\n", cpu->A, cpu->B);
    printf("MAR: 0x%x%x\n", cpu->MARHI, cpu->MARLO);
    printf("PC: 0x%x%x\n", cpu->PCHI, cpu->PCLO);
    printf("RAM[0x%x]: 0x%x\n", (cpu->MARHI << 8) | cpu->MARLO, cpu->RAM[(cpu->MARHI << 8) | cpu->MARLO]);
} // end coreDump

// Add a new IODevice to the internal list
void registerDevice(CPU* cpu, IODevice* device){
    cpu->devices[cpu->deviceCount] = *device;
    cpu->deviceCount = cpu->deviceCount + 1;
} // end registerDevice

// Debug print control lines
void dbgCtrlLine(byte CTRLWord){
    switch (CTRLWord & SRC) {
        case MOL:
            printf("MOL ");
            break;

        case MOH:
            printf("MOH ");
            break;

        case POL:
            printf("POL ");
            break;

        case POH:
            printf("POH ");
            break;

        case EO:
            printf("EO ");
            break;

        case AO:
            printf("AO ");
            break;
            
        case RD:
            printf("RD ");
            break;

        case CR:
            printf("CR ");
            break;
        
        default:
            printf("ERROR: Unimplemented/Invalid value 0x%x as bus source.\n", CTRLWord&SRC);
            break;
    }
    switch (CTRLWord & DST) {
        case II:
            printf("II ");
            break;

        case MIH:
            printf("MIH ");
            break;
            
        case MIL:
            printf("MIL ");
            break;
            
        case PIL:
            printf("PIL ");
            break;
            
        case PIH:
            printf("PIH ");
            break;
            
        case AI:
            printf("AI ");
            break;
            
        case BI:
            printf("BI ");
            break;
            
        case WR:
            printf("WR ");
            break;
        
        default:
            printf("ERROR: Unimplemented/Invalid value 0x%x as bus destination.\n", CTRLWord&DST);
            break;
    } 
    switch (CTRLWord & ALU) {
        case ADD:
            printf("ADD ");
            break;

        case RSA:
            printf("RSA ");
            break;

        case CE:
            printf("CE ");
            break;

        case INC:
            printf("INC ");
            break;

        default:
            printf("ERROR: Unimplemented/Invalid value 0x%x as ALU configuration.\n", CTRLWord&ALU);
            break;
    }
    printf("\n");
} // end dbgCtrlLines