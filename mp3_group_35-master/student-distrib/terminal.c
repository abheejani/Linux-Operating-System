#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "x86_desc.h"
#include "terminal.h"
#include "paging_asm.h"
#include "cursor.h"
#include "pit.h"


/*
 * Initialize terminal
 * 
 * Inputs: None
 * Outputs: None
 * Function: Initializes terminal-related variables and structures, including process control blocks and attributes.
 */


void terminal_init() {
    // Initializes terminal-related variables and structures

    // Initialize global counter for process control blocks (PCBs)
    global_pcb_counter = 0;

    int i;
    // Initialize process control block arrays and terminal-related variables for each terminal
    for (i = 0; i < 6; i++) {
        // Initialize PCB array for terminal 0
        pcb_array[i] = 0;
        // Initialize PCB arrays for terminals 0, 1, and 2
        terminal_array[0].pcb_array[i] = 0;
        terminal_array[1].pcb_array[i] = 0;
        terminal_array[2].pcb_array[i] = 0;
    }

    // Initialize terminal-specific attributes for three terminals
    for (i = 0; i < 3; i++) {
        // Initialize cursor position and buffer attributes for each terminal
        terminal_array[i].curr_x = 0;
        terminal_array[i].curr_y = 0;
        terminal_array[i].buffer_character_count = 0;
        terminal_array[i].terminal_id = i;
        terminal_array[i].buffer_index = 0;
        // Initialize process ID, turn on status for terminals
        terminal_array[i].pid = 0;
        terminal_array[i].turned_on = 0;
        // terminal_array[i].pcb_array[0]->global_pcb_value = i; 
    }

    // Set the current terminal ID to 0 and switch to terminal 0
    current_terminal_id = 0;
    terminal_switch(0);

    // Initialize target PID
    target_pid = 0;

    // Exit the function
    return;
}

///////////////////////ATTEMPT AT PROCESS SWITCH//////////////////////////////////////////////
void process_switch(uint32_t process_id) {
    // cli(); 
    // register uint32_t ebp_value asm("ebp"); // current vals
    // register uint32_t esp_value asm("esp");

    // int old = process_id - 1;
    // if (old < 0) {
    //     old = 2;
    // }
    // save_old(old, process_id, ebp_value, esp_value); 

    // program_pages(pcb_array[target_pid]->global_pcb_value);

    // counter = (counter + 1) % 3;
    // restore_new(process_id); 

    // sti(); 
}
///////////////////////////////////////////////////////////////

/*
 * Save old process state and swap in new process
 * 
 * Inputs: cur - Current terminal index
 *         new - New terminal index
 *         ebp_value - Value of the ebp register
 *         esp_value - Value of the esp register
 * Outputs: None
 * Function: Saves the state of the current process and swaps in the state of a new process.
 */

void save_old(int cur, int new, register uint32_t ebp_value, register uint32_t esp_value) {
    // get rid of pointers
    terminal_array[cur].esp = esp_value;
    terminal_array[cur].ebp = ebp_value;
    terminal_array[cur].ss = tss.ss0;
    terminal_array[cur].esp0 = tss.esp0;

    //terminal check, can only be between 0-2
    if (new > 2 || new < 0) {
        return;
    }

    //swapping out pcb array - between global and termimal pcb array 
    int i = 0;
    for (i = 0; i < 6; i++) {
        terminal_array[cur].pcb_array[i] = pcb_array[i]; // store cb array of what you are about to change from
        pcb_array[i] = terminal_array[new].pcb_array[i]; // store pcb array with what y9ou are going to
    }

    //swapping the target pid so it is for the most recent process of the terminal pcb 
    terminal_array[cur].pid = target_pid;
    target_pid = terminal_array[new].pid;

    return;
}


/*
 * Restore new process state
 * 
 * Inputs: new - New terminal index
 * Outputs: None
 * Function: Restores the state of a new process by updating ESP and EBP registers.
 */
void restore_new(int new) {
    cli();
    uint32_t ESP;
    uint32_t EBP;

    //restoring the ss0 and esp0 for new terminal to resume 
    // tss.ss0 = terminal_array[new].ss;
     tss.ss0 = KERNEL_DS;
    tss.esp0 = terminal_array[new].esp0;

    //restoring the esp and ebp for the terminal we're switching to 
    ESP = terminal_array[new].esp;
    EBP = terminal_array[new].ebp;

    //in line assembly to swtich back this terminala and resume its process
    asm volatile(
        "movl %0, %%esp \n"
        "movl %1, %%ebp \n"
        : /* no output */
        : "r"(ESP), "r"(EBP)
        : "memory");
    sti();

    return;
}

/*
 * Switch terminal
 * 
 * Inputs: terminal_id - Index of the terminal to switch to
 * Outputs: None
 * Function: Switches to the specified terminal, saving the state of the current process.
 */
void terminal_switch(uint32_t terminal_id) {
    //save the ebp and esp for this terminals process 
    register uint32_t ebp_value asm("ebp"); // currentm vals
    register uint32_t esp_value asm("esp"); 

    //call save old to store these and swap 
    save_old(current_terminal_id, terminal_id, ebp_value, esp_value);

    //saving the current keyboard buffer 
    strncpy(terminal_array[current_terminal_id].keyboard_buffer, keyboard_buffer, 128);
    terminal_array[current_terminal_id].buffer_index = keyboard_buffer_index;

    //saving the cursor for the current terminal 
    terminal_array[current_terminal_id].curr_x = screen_x;
    terminal_array[current_terminal_id].curr_y = screen_y;

    // which t4erminal to switch to
    int old_addr = (185 + current_terminal_id) << 12; //185 is the start of video memory, adding the id to store in adjacent backup - is xB9
    int addr = (185 + terminal_id) << 12;


    //switching video memory so that we see the new termianl 
    send_eoi(1);
    memcpy((uint8_t *)old_addr, (uint8_t *)0xB8000, 4096); //xB8000 is start of video memory and 4096 is 4kb which is the size of the vid mem page 
    memcpy((uint8_t *)0xB8000, (uint8_t *)addr, 4096);

    //putting the keyboard saved in the terminal into the global one 
    strncpy(keyboard_buffer, terminal_array[terminal_id].keyboard_buffer, 128);
    keyboard_buffer_index = terminal_array[terminal_id].buffer_index;

    //putting the saved cursor in the terminal into the global one 
    screen_x = terminal_array[terminal_id].curr_x;
    screen_y = terminal_array[terminal_id].curr_y;
    update_cursor(screen_x, screen_y);

    //now chaning the current terminal id to be the new one 
    current_terminal_id = terminal_id;

    //if the termianl hasnt been opened yet, execute shell on it to initialize 
    if (terminal_array[current_terminal_id].turned_on == 0) {
        
        terminal_array[current_terminal_id].turned_on = 1;

        execute((const uint8_t *)"shell");
    }

    //copy into program image to allow process to run 
    program_pages(pcb_array[target_pid]->global_pcb_value);

    //flush tlb 
    asm volatile(" \
        movl %%cr3, %%eax ; \
        movl %%eax, %%cr3 ; \
    " : /* no outputs */ \
    : /* no inputs */ \
    : "eax", "cc");

    // counter = (counter + 1) % 3;

    //now restore terminal process 
    restore_new(terminal_id);
}
