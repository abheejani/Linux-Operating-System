#include "rtc.h"
#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"

// Commands
#define RESET               0xFF            // Command to reset
#define ENABLE_SCANNING     0xF4            // Command to enable scanning

// Responses
#define ACK                 0xFA            // Response: Acknowledgment
#define RESEND              0xFE            // Response: Re-send request
#define TEST_PASSED         0xAA            // Response: Test passed
#define TEST_FAILED         0xFC            // Response: Test failed
#define TEST_FAILED_2       0xFD            // Response: Test failed (alternative)

//ports-apparently these are convention for PS2 keyboard
#define COMMAND_PORT 0x64   // COMMAND_PORT: Port address for command communication with the keyboard controller
#define DATA_PORT 0x60      // DATA_PORT: Port address for data communication with the keyboard controller

/* Real-Time Clock (RTC) Initialization Function
 * Inputs: None
 * Outputs: None
 * Function: Initializes the RTC by configuring its control registers.
 *           It sets up the RTC to generate interrupts on IRQ 8 (RTC interrupt).
 *           This function should be called during system initialization. */
void init_RTC() {
    //cli();
    outb(0x8B,0x70); // Select Register B and disable NMI forsureskis. 0x8B =  Register B && 0x70 = used to specifiy index and disable NMI
    char prev = inb(0x71); // Read the current value of Register B forsureskis. 0x71 = Used to read or write to from that byte of CMOS configuration spcae (PORT)
    outb(0x8B, 0x70); // Set the index again forsureskis. 0x70 = used to specifiy index and disable NMI. 0x8B is register B 
    outb(prev | 0x40, 0x71); // Enable bit 6 of Register B to turn on IRQ 8 forsureskis
    

    outb(0x8A, 0x70);		// set index to register A, disable NMI. Reg A is 8A and 70 is the index
    prev=inb(0x71);	// get initial value of register A. 71 is used to read or write to and form 
    outb(0x8A, 0x70);		// reset index to A. Index is 70 and 8A is regsiter  
    enable_irq(8);  //8th Interrupt 

    // //sti();
}



/* Real-Time Clock (RTC) Interrupt Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles RTC interrupt. If the 'flag' is set, it calls 'test_interrupts' function.
 *           Reads register C of the RTC to clear the interrupt and acknowledges the interrupt. */
void rtc_interrupt() {
    //flag = 1; 
   if (flag) {
       // test_interrupts();  // Call 'test_interrupts' function if the 'flag' is set.
        printf("1");
    }
    outb(0x0C, 0x70);  // Select register C of the RTC. x70 is the index
    inb(0x71);         // Read register C to clear the interrupt. this is the read or write port is 0x71
    read_flag = 1; 
    send_eoi(8);        // Send an End-of-Interrupt signal to acknowledge the interrupt. interrupt numbr 8 is the rtc
}

/* RTC Write Function
 *
 * This function sets the update rate of the Real-Time Clock (RTC) chip.
 * The update rate determines how often the RTC generates interrupts. The provided
 * frequency should be a power of 2 between 2 and 1024 Hz.
 *
 * Inputs:
 *   - freq: The desired RTC update frequency (in Hz).
 *
 * Outputs:
 *   - Returns 4 if the frequency is set successfully, or -1 if the provided
 *     frequency is not a power of 2 between 2 and 1024 Hz.
 */

 int rtc_write(int32_t fd, void* buf, int32_t nbytes){
        uint32_t* int_buf = (uint32_t*) buf; 
        int freq = *int_buf;
        if ((freq & (freq - 1)) != 0 || freq > 1024){ //chekcing that freqeuncy is non negative and that it is not over the max 1024 Hz
            return -1;                                  //if so then retunr -1 for failiure
        }

    int rate = 0;                                         //initialize rate to 0 
    int temp = 32768/freq;                                  //base frequency of the RTC's oscillator in Hertz
    //printf("temp: %d", temp);
    //log base 2
     while (temp >= 1) {      // This loop continues as long as the 'temp' value remains greater than or equal to 1.                               
        temp /= 2;             // Within the loop, the 'temp' value is halved (divided by 2) in each iteration.
        rate += 1;              // The 'rate' variable is incremented by 1 in each iteration of the loop, effectively counting the number of times 'temp' is halved.
        // printf("temp: %d", temp);
        // printf("rate: %d", rate);
    }

    //rate++; 
    //printf("rate: %d", rate);
    //Select Register A and disable the NMI
    outb(0x8A, 0x70); //index at port 70
    // Get the initial value of Register A
    char prev = inb(0x71);      //data port 71 reading from there
    // Reset the index to Register A
    outb(0x8A, 0x70); //index at port 70
    // Write the new rate to Register A
    outb((prev & 0xF0) | rate, 0x71);  //F0 masks getting anded with prev then set at data port 71

    return 4;                           //return 4 for bytes returned 

 }


/* Open the Real-Time Clock (RTC)
 *
 * This function is responsible for opening the RTC and configuring it to generate
 * periodic interrupts at a specific frequency (2 Hz in this case).
 *
 * Inputs: None
 * Outputs: 0 if the RTC is successfully opened
 */
int rtc_open(uint8_t* filename){
    //rtc_write(2); //change frequency to 2 Hz
    int freq = 2; 
    int rate = 0;                                         //initialize rate to 0 
    int temp = 32768/freq;                                  //base frequency of the RTC's oscillator in Hertz
    //printf("temp: %d", temp);
    //log base 2
     while (temp >= 1) {      // This loop continues as long as the 'temp' value remains greater than or equal to 1.                               
        temp /= 2;             // Within the loop, the 'temp' value is halved (divided by 2) in each iteration.
        rate += 1;              // The 'rate' variable is incremented by 1 in each iteration of the loop, effectively counting the number of times 'temp' is halved.
        // printf("temp: %d", temp);
        // printf("rate: %d", rate);
    }

    //rate++; 
    //printf("rate: %d", rate);
    //Select Register A and disable the NMI
    outb(0x8A, 0x70); //index at port 70
    // Get the initial value of Register A
    char prev = inb(0x71);      //data port 71 reading from there
    // Reset the index to Register A
    outb(0x8A, 0x70); //index at port 70
    // Write the new rate to Register A
    outb((prev & 0xF0) | rate, 0x71);  //F0 masks getting anded with prev then set at data port 71

    //return 4;   
    return 0; 
}


/* Read from the Real-Time Clock (RTC)
 *
 * This function is responsible for reading data from the RTC. In this minimal
 * implementation, it always returns 0 as reading from the RTC is not implemented.
 *
 * Inputs: None
 * Outputs: Always returns 0 in this implementation
 */
int rtc_read(int32_t fd, void* buf, int32_t nbytes) {

    read_flag = 0; 
    while (!read_flag); 
    return 0;
}
//return 0


/* Close the Real-Time Clock (RTC)
 *
 * This function is responsible for closing the RTC. In this minimal
 * implementation, it always returns 0 as closing the RTC is not implemented.
 *
 * Inputs: None
 * Outputs: Always returns 0 in this implementation
 */

int rtc_close(int32_t fd){
    return 0;
}

int32_t (*rtc_jump_table[4]) () = {rtc_read, rtc_write, rtc_open, rtc_close};
