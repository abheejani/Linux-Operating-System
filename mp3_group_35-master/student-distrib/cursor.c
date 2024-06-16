#include "cursor.h"
#include "lib.h"



/*
 * init_cursor - Initialize the text mode cursor.
 *
 * This function initializes the text mode cursor settings. It sets the cursor
 * start and end lines to zero to hide the cursor initially and positions the
 * cursor at the top-left corner of the screen (row 0, column 0).
 *
 * Inputs: None
 * Outputs: None (void)
 */
void init_cursor() {
    // Initialize the cursor start and end to zero to hide the cursor initially
    outb(0x0A, 0x3D4);  //Index register A at port 3D4 for start line
    outb(0, 0x3D5);    //write to datat port 3D5 with 0 to make start line 0

    outb(0x0B, 0x3D4);  // Index to register B at port 3D4. Set cursor end line.
    outb(30, 0x3D5);    //Set end line at row 30 but data port is 3D5

    // Set the cursor location to top-left (row 0, column 0)
    outb(0x0F, 0x3D4);  // Write to Register F to port 3D4. High byte of cursor location
    outb(0, 0x3D5); //Set 0 to data port 3D5. vert, row

    outb(0x0E, 0x3D4);  // Write to register 0E and port 3D4. Low byte of cursor location
    outb(0, 0x3D5); //rite 0 to data port. horiz, col
    
}



/*
 * disable_cursor - Disable the text mode cursor.
 *
 * This function disables the text mode cursor by setting both the start and
 * end scanlines to 0, effectively hiding the cursor.
 *
 * Inputs: None
 * Outputs: None (void)
 */
void disable_cursor() {
    // Disable the cursor by setting both start and end scanlines to 0
    outb(0x0A, 0x3D4);  //A is regsiter and port is 3D4  Set cursor start line-this is a boundary
    outb(0x20, 0x3D5); // Set the start to 20 at data port 3D5
}


/*
 * update_cursor - Set the text mode cursor location.
 *
 * This function sets the text mode cursor location to the specified coordinates
 * (x, y) on the screen using VGA text mode. The cursor position is calculated
 * in row-major order, where 'x' represents the screen's horizontal position
 * (column) and 'y' represents the vertical position (row).
 *
 * Inputs:
 *   - x: The horizontal position (column) for the cursor.
 *   - y: The vertical position (row) for the cursor.
 *
 * Outputs: None (void)
 */
void update_cursor(int x, int y) {
    // Set the cursor location
   unsigned short position = y * VGA_WIDTH + x; //Row Major order for current position on the screen, where x = screenx and y= screen y 
    outb(0x0F, 0x3D4);  //low byte using register F at port 3D4
    outb((unsigned short) (position & 0xFF), 0x3D5); //. Bit mask with FF at the high bytes. Set the new position with the position and 3D5 as the data port 
    outb(0x0E, 0x3D4);  //high byte using Register E at port 3D4 
    outb((unsigned short) ((position >> 8) & 0xFF), 0x3D5); // get low byte by using bit mask and shifting by 8 And anding with FF at data port 3D5
}


