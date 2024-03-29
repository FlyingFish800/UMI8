// Alexander Symons | Aug 6 2022 | Emulator | Main.c
// Driver file for the CPU
// Inputs: microcode binary
#include "CPU.h"
#include "IODevices/terminal.h"
#include "IODevices/ansiicommands.h"

#define THREADED
//#define DEBUG

#ifdef THREADED
#warning "Multithreading on"
#include <pthread.h>
#endif

CPU core;
IODevice dummy;

//int number(){
//    return 0x42;
//}

#ifdef THREADED
char core_thread_running = 1;
int clocks = 0;

char transfer_char = 0;

pthread_mutex_t run_status_lock;
pthread_mutex_t core_data_access;

// Run in a seperate thread so terminal IO doesnt halt program
void *run_core_threaded(void *vargp){
    char running = 1;
    while (running){
        pthread_mutex_lock(&core_data_access);
        clock_core(&core);
        pthread_mutex_unlock(&core_data_access);

        pthread_mutex_lock(&run_status_lock);
        running = core_thread_running;
        pthread_mutex_unlock(&run_status_lock);
        clocks++;
    }
    printf("Core killed\n");
}
#endif

int main(int argc, char *argv[]){
    dummy.address = 0xFEFF;
    dummy.readDevice = NULL;
    dummy.writeDevice = sendChar;


    reset(&core);
    
    loadUCode(&core, "microcode.bin");

    registerDevice(&core, &dummy);
    
    loadRam(&core, "../Assembler/a.bin");
    core.RAM[0xFFFF] = 0xFE;

    for (int i = 0; i < 16; i++) {
        printf("0x%02x: 0x%02x\n", i, core.RAM[i]);
    }

    #ifndef THREADED
    
    char c = 0x0;
    while (c != 'q') {
        // UCode index and step mismatch because clock updates step at end
        c = getchar();
        core.RAM[SERIAL_OUT] = c;

        if (c == 'd'){
            for (int i = 0; i < RAM_SIZE; i++){
                if (core.RAM[i] != 0) printf("0x%x: 0x%x\n", i, core.RAM[i]);
            }
        } else if (c == 'c') {
            while(1) {
                clock_core(&core);
                if (&core.IR == 0) break;
            } 
        } else {
            clock_core(&core);
        }

        #ifdef DEBUG
        coreDump(&core);
        #endif
    }

    #else

    if (pthread_mutex_init(&run_status_lock, NULL) != 0){
        printf("Failed to init lock!\n");
        return -1;
    }

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, run_core_threaded, NULL);

    ANSIRAW();

    char c = 0x0;
    while (c != 'q') {
        c = getchar();
        printf("%c\n", c);
        pthread_mutex_lock(&core_data_access);
        core.RAM[SERIAL_OUT] = c;
        pthread_mutex_unlock(&core_data_access);


        #ifdef DEBUG
        pthread_mutex_lock(&core_data_access);
        coreDump(&core);
        pthread_mutex_unlock(&core_data_access);
        #endif
    }

    pthread_mutex_lock(&run_status_lock);
    core_thread_running = 0;
    pthread_mutex_unlock(&run_status_lock);

    pthread_join(thread_id, NULL);

    ANSICOOKED();

    printf("Clocks: %d\n", clocks);

    #endif
    
} // end main