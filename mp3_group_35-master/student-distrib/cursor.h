#ifndef CURSOR_H
#define CURSOR_H

#define VGA_WIDTH 80


void init_cursor();
// Function to enable the cursor with custom start and end scanlines
void enable_cursor(unsigned char cursor_start, unsigned char cursor_end);
void disable_cursor();
void update_cursor(int x, int y);
#endif // CURSOR_H

