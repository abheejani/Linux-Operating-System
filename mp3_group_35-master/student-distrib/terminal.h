


#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "types.h"
#include "tests.h"
#include "System_Calls.h"

typedef struct terminal {
  uint32_t curr_x;          //x current cursor position 
  uint32_t curr_y;          //y current cursor position 
  uint32_t buffer_character_count;   //the buffers current count 128 max
  char keyboard_buffer[128];          //buffers current state 
  uint32_t terminal_id;
   int buffer_index;
   int active_flag; //if its the current terminal (not rlly necesary)
  int turned_on; 
  int pid; // current process' pid
  pcb_t* pcb_array[6]; //have onen pcb array per terminal, make active when active
  uint32_t esp; 
  uint32_t ebp; 
  uint16_t ss; 
  uint8_t eip; 
  uint32_t esp0;
  int global_pcb_index; //so we know what we are paging w
  int global_pcb_value;
  volatile int enter_flag; 
} terminal_t;


uint32_t current_terminal_id; //global current terminal running (like target_pid)

terminal_t terminal_array[3]; //3 instances of the struct for 3 terminals 

void terminal_init();
void terminal_switch (uint32_t terminal_id); //pass in the terminal 
void save_old(int cur, int new, register uint32_t ebp_value,register uint32_t esp_value);
void restore_new(int new); 
void process_switch ();

//int pid_of_terminals_array[3]; //tells what the parent pid for each parent shell is





#endif /* _TERMINAL_H */

