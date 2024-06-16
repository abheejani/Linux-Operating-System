


#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "System_Calls.h"
#include "terminal.h"
 
int ctrl_flag = 0; 
int shift_flag = 0; 
int capslock_flag = 0;
int alt_flag = 0; 
int kb_flag = 0; 
//int terminal_array[current_terminal_id].enter_flag = 0; 
/* Initialize the Real-Time Clock (RTC)
 *
 * This function initializes the Real-Time Clock (RTC) for system timekeeping.
 * It configures the RTC's Register B to enable RTC interrupts on IRQ 8, which
 * allows the system to receive periodic timekeeping updates.
 *
 * Inputs: None
 * Outputs: None
 */
void init_keyboard(){
enable_irq(1); //enable interrupt 1
}



/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 

unsigned char keyScanCodeArray_one[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '\0', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   

/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 

unsigned char keyScanCodeArray_for_shift[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '\0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   


/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 

unsigned char keyScanCodeArray_for_just_capslock[0x59] = {'\0','\0','1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
'=', '\0', '\0', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', '\0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`','\0', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};   


/* Array that maps keyboard scan codes to characters and control codes.
 * Each element in the array represents the character or control code associated with a specific scan code.
 * Note: Some entries are marked as '\0', which means they have no corresponding character or control code.
 * The array provides a lookup table for translating scan codes into readable characters. */
//0x59 is limit on array whihch is 89 in decimal 

unsigned char keyScanCodeArray_for_shift_and_capslock[0x59] = {'\0','\0','!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', 
'+', '\0', '\0', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', '\0', '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 
'\0', '*', '\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7', '8', 
'9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\0', '\0', '\0', '\0', '\0'};  



/* Keyboard Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles keyboard interrupt, reads the scan code from the keyboard controller, 
 *           maps it to a character using keyScanCodeArray_one, and prints the corresponding character. */
int keyboard_interrupt() {
    // keyboard_buffer_index = 0; 
    // keyboard_buffer = ''; 
    // char* temp_buf; //this is what well write with 
    scan_code = inb(0x60);  // Read the scan code from the keyboard controller. x60 is the keyboard data port 
    send_eoi(1);  
    // write(keyboard_buffer, 0);  //call write to write to the buffer
    // read(keyboard_buffer, 0); //prints buffer to screen on enter 
    if (alt_flag == 1){
        if (scan_code == 0xBB && current_terminal_id !=0){
            terminal_switch(0);
            send_eoi(1); 
            // /execute("shell");
            return 0;
        }
         if (scan_code == 0xBC && current_terminal_id !=1){
            terminal_switch(1);
            send_eoi(1); 
            return 0;
        }
        if (scan_code == 0xBD && current_terminal_id !=2 ){
            terminal_switch(2);
            send_eoi(1); 
            return 0;
        }
    }
    
     if (scan_code == 0x3A){//Caps lock key code 
        capslock_flag += 1; // Increment the capslock_flag by 1.
        capslock_flag %= 2;  // Use modulo to ensure capslock_flag stays within the range of 0 or 1.
        send_eoi(1);  
        return 0;                //return from function 
    }
    if(scan_code == 0x1D){  //if control is pressed then set flag to 1 
        ctrl_flag = 1;  //if control is pressed then set flag to 1 
        send_eoi(1);  
        return 0;
    }
    if (scan_code == 0x9D){    //if CONTROL IS released bc 9d is key released code for control 
        ctrl_flag = 0;             // set control flag to 0 
        send_eoi(1);  
        return 0; 
    }
    if (ctrl_flag==1 && scan_code == 0x26){//this is for ctrl+L, we'll need to make arrays for ctrl and shifted characters. L is keycode 0x26
        clear(); //call clear function to clear screen
        init_buffer(); //reinitalize th ebuffer
        send_eoi(1);  
        return 0; 
    }
    // if (scan_code == 0x1C){
    //     read(NULL, keyboard_buffer, keyboard_buffer_index);
    // }

    if (ctrl_flag)                  //if control is pressed return bc it does nothing for now
        {
            send_eoi(1);  
        return 0; 
        }
    if (scan_code == 0x0E) { // Backspace is keycode 0E
    if (keyboard_buffer_index == 0){
        send_eoi(1);  
        return 0; 
    }
    int i;
    int backspace_count = 0;            //set back space coutn to 0 to intialize it 

    for (i = 0; i < 4; i++) {           //perform 4 backspaces for a tab
        if (keyboard_buffer_index > 0 && keyboard_buffer[keyboard_buffer_index - 1] == ' ') {
            backspace();
            keyboard_buffer_index--;        //subtract the buffe rindex 
            backspace_count++;              //increase backspace count 
        } else {
            break;
        }
    }

    // If no tab spaces were removed, perform a regular backspace
    if (backspace_count == 0) {
        backspace();            
        if (keyboard_buffer_index > 0) {
            keyboard_buffer[keyboard_buffer_index - 1] = '\0';         //print regular null character for regular back space and put into buffer 
            keyboard_buffer_index--; //subtract the buffer index when back space is clicked 
        }
    }
    send_eoi(1);  
    return 0;
}
//ali
if (scan_code == 0x0F) { // Tab is keycode 0F.
    int i;
    for (i = 0; i < 4; i++) {                           //if tab is pressed then print 4 spaces
                                                        // Update the buffer with four spaces
        if (keyboard_buffer_index < 127) {                  //128 character limit
            keyboard_buffer[keyboard_buffer_index] = ' ';     //putting space into buffer
            putc(' ');                                      // Print a space character            
           
            keyboard_buffer_index++;                        //increase buffer indez
        }
    }
    send_eoi(1);  
    return 0;
}





    ///////SHIFT////////////
    if(scan_code == 0xAA || scan_code == 0xB6){//left shift releases is AA and B6 is right shift release 
       shift_flag = 0;                              //set shift flag to zero 
    }
    if(scan_code == 0x2A || scan_code == 0x36){//shift pressed left shif tis 2A and right shift is 36 
        shift_flag = 1;                         //set flag to 1
    }

    //////Alt
    if(scan_code == 0xB8){//released            Alt key release is key B8
       alt_flag = 0; 
    }
    if(scan_code == 0x38){//pressed ALT KEY pressed is x38 scancode 
        alt_flag = 1;
    }
    //used to test alt flag works when alt + l pressed bc idrk what alt does yet
    // if (alt_flag == 1 && scan_code == 0x26){
    //     printf("alt flag works");
    // }


    else if (scan_code < 89 && scan_code != 0x1C) { //array index is less than 89 and ENTER NOT PRESSED BC ENTER = 1C
    if(keyboard_buffer_index >= 127 || keyScanCodeArray_one[scan_code] == '\0'){ // If buffer is full >= 127. 128 character limit
        send_eoi(1);  
        return 0; 
    }
        if(shift_flag == 1 && capslock_flag == 0){  //if shift is pressed but not capslock 
            char output = keyScanCodeArray_for_shift[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; 
            keyboard_buffer_index ++;
            putc(output);
            //write (output, 1);
        }
        else if (shift_flag == 1 && capslock_flag == 1){            //handling cas eif shift and capslock pressed at same time
            char output = keyScanCodeArray_for_shift_and_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; //writing to buffer
            keyboard_buffer_index ++;
            putc(output);
             //write (output, 1);
        }
         else if (shift_flag == 0 && capslock_flag == 1){       //if capslock is pressed but not shift 
            char output = keyScanCodeArray_for_just_capslock[scan_code];  // Map the scan code to a character.
            keyboard_buffer[keyboard_buffer_index] = output; //writing to buffer
            keyboard_buffer_index ++;
            putc(output);
             //write (output, 1);
        }
        else{
        char output = keyScanCodeArray_one[scan_code];  // Map the scan code to a character.
        keyboard_buffer[keyboard_buffer_index] = output; //writing to buffer
        keyboard_buffer_index ++; 
        putc(output);  // SHOULD BE PUTC
         //write (output, 1);
        }
}
    
    
    //write(keyboard_buffer, 1); //write character to screen then reset buffer
    //init_buffer(); 
    send_eoi(1);                //end of interrupt

    return scan_code;           //returns the scan code
}




/*
 * write - Process and display keyboard input.
 *
 * This function processes keyboard input, including handling control keys (Ctrl, Shift, Caps Lock)
 * and special keys (Tab, Backspace), and writes characters to a keyboard buffer and displays
 * them on the screen.
 *
 * Inputs:
 *   - buf: The buffer for storing keyboard input.
 *   - nbytes: The number of bytes to write (not used in this function).
 *
 * Outputs: None (void)
 */
 int32_t write (int32_t fd, const void* buf, int32_t nbytes){//need to add fd as a function once file system worknig
  cli(); 
   if (fd != 1){
    return -1; 
   }
   
   if(nbytes < 0){
    return -1;
   }
   
   int i; 
   //printf("nbytes in write: %d\n", nbytes);
   const char* new_buf = (const char*)buf;
   for (i = 0; i < nbytes; i++){
    putc (new_buf[i]);
   }
   
   sti(); 
   return nbytes; 
}


/* Read function for handling keyboard input
 *
 * This function is called when reading input from the keyboard. It checks if
 * the input corresponds to an "Enter" key press (scan code 0x1C), and if so,
 * it prints the contents of the keyboard buffer to the screen, followed by
 * a newline character. The keyboard buffer is then reset for the next input.
 *
 * Inputs:
 * - buf: A pointer to a character buffer (not used in this function)
 * - nbytes: The number of bytes to read (not used in this function)
 *
 * Outputs:
 * - Returns 0 to indicate success (keyboard input handled)
 */
 int32_t read(int32_t fd, void* buf, int32_t nbytes){//need to add fd as a function once file system worknig
        //printf("terminal read");
    //  if (scan_code != 0x1C){        //if eneter is not pressed return and do nothing 
    //     return 0; 
    //  }
    //printf("read");
    // if (scan_code == 0x9C){
    //         terminal_array[current_terminal_id].enter_flag = 0; 
    //     } 
    // if (terminal_array[current_terminal_id].enter_flag == 1)
    
    
    // int i; 
    // for (i = 0; i< 128; i++){
    //     buf[i] = '\0';
    // }
   // printf("nbytes: %d\n", nbytes);
   if (fd != 0){
    return -1; 
   }
    if (nbytes > 128){
        nbytes = 128; 
    }
    //printf("nbytes: %d\n", nbytes);
    while (terminal_array[current_terminal_id].enter_flag == 1){
        if (scan_code == 0x9C){
            terminal_array[current_terminal_id].enter_flag = 0; 
            break; 
        }
            
    }
        
    while (scan_code != 0x1C); 
     //enter case             //if enter is pressed print buffer
       
                             //print a new line 
        // int i; 
        // for (i = 0; i<keyboard_buffer_index; i++){      //print each keyboard index bc a characte ris store din eahc index 
        //     putc(keyboard_buffer[i]);
        // }
        //             //print new line after printing the whole buffer 
        // init_buffer();      //reinitialize the buffer 
        // return 0; 
    if (scan_code == 0x1C && terminal_array[current_terminal_id].enter_flag != 1){
         //char* new_buf = (char*)buf;
    putc('\n'); 
    // for (i = 0; i< nbytes; i++){
    //     printf("i: %d\n", i);
    //     //new_buf[i] = keyboard_buffer[i];
    //     *(uint8_t*)(buf + i) = keyboard_buffer[i];
    // }
    //printf("yippekaiya");
    //write(NULL, buf, 0);
    //putc('\n'); 
    memcpy(buf, keyboard_buffer, nbytes);
    //printf("left for loop");
    terminal_array[current_terminal_id].enter_flag = 1; 
    }
    init_buffer(); 
    //printf("left read");
     return nbytes;

}

/* Initialize the keyboard input buffer
 *
 * This function initializes the keyboard input buffer by setting all its
 * elements to null characters ('\0'). It also resets the buffer index to zero.
 *
 * Inputs: None
 * Outputs: None (void)
 */
void init_buffer(){
    int i; 
    for (i = 0; i< 128; i++){           //iterate through each of the 128 character limit to initialize them to null
        keyboard_buffer[i] = '\0';
    }
    keyboard_buffer_index = 0;              //reset the keyboar dindex to zero 
}

/* Terminal Open Function
 *
 * This function is a placeholder for the open operation of a terminal device,
 * and it returns 0 to indicate success.
 *
 * Inputs: None
 * Outputs: Returns 0 to indicate success
 */
int32_t terminal_open(uint8_t* filename){
    return 0;
}


/* Terminal Close Function
 *
 * This function is a placeholder for the close operation of a terminal device,
 * and it returns 0 to indicate success.
 *
 * Inputs: None
 * Outputs: Returns 0 to indicate success
 */
int32_t terminal_close(int32_t fd){
    return 0;
}

int32_t (*terminal_jump_table[4]) () = {read, write, terminal_open, terminal_close};

