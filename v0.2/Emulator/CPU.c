// Contains implementation of UMI 8 CPU
#include "CPU.h"

#ifdef DEBUG
#warning "Debug on"
#endif

// LUT for instruction names
const char *INSTRUCTION[] = {"NOP",       "CALL I",   "RET",      "PSH R",    "POP R",    "",     "",     "", 
                            "LD R I",    "LD R MA",  "LD MA R",  "LD R MR",  "LD MR R",  "",     "",     "",
                            "JP I",      "JPZ I",    "JNZ I",    "JPC I",    "PNC I",    "JPN I","JNN I","",
                            "ADD I",     "ADDC I",   "INC R",    "ZRO R",    "SUB I",    "CMP I","",     "LOGIC I"};

// Reset all internal registers and state of CPU.
// DOESN'T RESET MICROCODE
void reset(CPU* cpu){
    cpu->A = 0;
    cpu->B = 0;
    cpu->C = 0;
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
void clock_core(CPU* cpu){
    // Figure out ctrl byte (Instruction register is &|!OOOOO where first 3 are loadA modifiers, rest are opcode)
    // Ucode indexing is formatted IIIIIFFF0SSSS where S = UStep, I = instruction, F = flags (ZNC)
    unsigned short MCIndex = ((cpu->IR & 0b11111) << 8) | ((cpu->Flags & 0b111) << 5) | (cpu->UStep & 0b01111);
    byte CTRLWord = cpu->Microcode[MCIndex];

    // Print ctrl info for debugging
    #ifdef DEBUG
    char flags_status[4] = "   \0";
    if ((cpu->Flags & ZERO) == ZERO) flags_status[0] = 'Z';
    if ((cpu->Flags & NEGATIVE) == NEGATIVE) flags_status[1] = 'N';
    if ((cpu->Flags & CARRY) == CARRY) flags_status[2] = 'C';
    flags_status[3] = '\0';
    printf("UCode Index: %d   Flags: %s   Ctrl Word: 0x%x ", MCIndex, flags_status, CTRLWord);
    dbgCtrlLine(CTRLWord);
    #endif

    // Every other bit of microcode being zero would load II with 0, a NOP
    if ((CTRLWord & ALU) == CR) {cpu->UStep = 0; return;}

    // Set active register based on REG SELECT line. TODO: unimplemented
    cpu->REG_ACTIVE = ((cpu->IR & RSEL) == RSEL) ? &cpu->C : &cpu->A;
    byte reg_val = *cpu->REG_ACTIVE;

    // Output to bus
    // Switch on SRC bits for ctrl word. Push appropriate value to bus
    byte flags = 0;
    switch (CTRLWord & SRC) { // TODO: EO 
        case BO:
            cpu->BUS = cpu->B;
            break;

        case POL:
            cpu->BUS = cpu->PCLO;
            break;

        case POH:
            cpu->BUS = cpu->PCHI;
            break;

        case EOS:
            // TODO: Fix flags
            // Add 1 if inc, also determine carry
            if((CTRLWord & ALU) == INC) {cpu->BUS = (reg_val + (~cpu->B & 0xFFFF) + 1) & 0xFFFF;if((reg_val+(~cpu->B & 0xFFFF)+1)>255) flags|=CARRY;}
            else {cpu->BUS = (reg_val + (~cpu->B & 0xFFFF)) & 0xFFFF;if(reg_val+(~cpu->B & 0xFFFF)>255) flags|=CARRY;}
            if(cpu->BUS == 0) flags |= ZERO;   // Zero
            if((cpu->BUS&0b10000000)>>7 == 1) flags |= NEGATIVE;   // Negative
            cpu->Flags = flags;
            break;

        case EO:
            // Add 1 if inc, also determine carry
            if((CTRLWord & ALU) == INC) {cpu->BUS = reg_val + cpu->B + 1;if((reg_val+cpu->B+1)>255)flags|=CARRY;}
            else {cpu->BUS = reg_val + cpu->B;if((reg_val+cpu->B)>255)flags|=CARRY;}
            if(cpu->BUS == 0) flags |= ZERO;   // Zero
            if((cpu->BUS&0b10000000) == 1) flags |= NEGATIVE;   // Negative
            cpu->Flags = flags;
            break;

        case AO:
            cpu->BUS = *cpu->REG_ACTIVE;
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
        
        case NO:    // No Out: Bus driven by INC. All ones if INC, else all 0s
            cpu->BUS = ((CTRLWord & ALU) == INC) ? 0xFF : 0x00;
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
            *cpu->REG_ACTIVE = cpu->BUS;
            break;
            
        case BI:
            #ifdef DEBUG
            printf("B: 0x%x BEFORE |%x &%x\n", cpu->B, ((cpu->IR&LDOR) == LDOR), ((cpu->IR&LDAND) == LDAND));
            #endif

            if ((cpu->IR&LDOR) == LDOR) cpu->B = reg_val | cpu->B;
            if ((cpu->IR&LDAND) == LDAND) cpu->B = reg_val & cpu->B;
            #ifdef DEBUG
            printf("B: 0x%x AFTER\n", cpu->B);
            #endif
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
                        printf("Device @[0x%x] write (0x%x/%c)\n", cpu->devices[i].address, cpu->BUS, cpu->BUS);
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
            cpu->PCHI++; // Wraps around to 0 from 255
            cpu->PCLO = 0;
        } // end if

        // Same for MAR bc of CEME
        if (cpu->MARLO < 255) { // Increment least significant
            cpu->MARLO++;
        } else { // Ripple carry
            cpu->MARHI++; // Wraps around to 0 from 255
            cpu->MARLO = 0;
        } // end if
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
    if (file == NULL) printf("UNABLE TO LOAD %s TO RAM\n", path);
    fread(cpu->Microcode, 1, MCODE_SIZE, file);
} // end loadRam

// Print status of cpu internal registers
void coreDump(CPU* cpu){
    byte active_reg = ((cpu->IR & RSEL) == RSEL) ? 'C' : 'A';
    printf("CURRENT INSTRUCTION: %s\n", INSTRUCTION[cpu->IR & 0b11111]);
    printf("IR: 0x%x  Flags: %d  Step: %d\n", cpu->IR, cpu->Flags, cpu->UStep);
    printf("Bus value: 0x%x\n", cpu->BUS);
    printf("A: 0x%x C: 0x%x  B: 0x%x   Active:%c\n", cpu->A, cpu->C, cpu->B, active_reg);
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
        case BO:
            printf("BO ");
            break;

        case EOS:
            printf("EOS ");
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
            printf("RO ");
            break;
            
        case RD:
            printf("RD ");
            break;

        case NO:
            printf("NO ");
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
            printf("RI ");
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

        case CR:
            printf("CR ");
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