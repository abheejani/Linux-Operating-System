#ifndef _SYSTEM_CALLS_H
#define _SYSTEM_CALLS_H

#include "tests.h"
#include "file_system.h"

// Define the start of the user stack
#define START_OF_USER_STACK 0x8400000

// Define the structure for Process Control Block (PCB)
typedef struct pcb {
    // Process ID
    uint8_t* esp_for_scheduling;
    uint8_t* ebp_for_scheduling;
    uint32_t pid;
    int state;
    uint8_t* sp;
    int inode_num;
    uint8_t* prev_ebp;
    uint8_t* prev_esp;
    void* pc;
    file_descriptor_struct_t fd_array[8];
    uint8_t args[128]; // Add an array to store command-line arguments
    int parent_pid; 
    int global_pcb_value;
    
} pcb_t;

// Array of PCBs to store process information
pcb_t* pcb_array[6];

// Pointer to the PCB of the currently targeted process
pcb_t* create_pcb(uint32_t pid);

// Process ID of the currently targeted process
uint32_t target_pid;

// Flag to indicate if file descriptors are full
int fd_full;

int global_pcb_counter; 

// System call functions

int32_t halt(uint8_t status); 
int32_t execute(const uint8_t* command);
int32_t read_syscall(int32_t fd, void* buf, int32_t nbytes);
int32_t write_syscall(int32_t fd, const void* buf, int32_t nbytes);
uint32_t open_syscall(uint8_t* filename);
int32_t close_syscall(int32_t fd);
uint8_t my_pids[6];
uint8_t get_new_pid();
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn();

#endif /* _SYSTEM_CALLS_H */
