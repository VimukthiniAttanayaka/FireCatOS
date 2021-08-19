#include "serial.h"
#include "io.h"
#include "Memory_segment.h"

char *fb = (char*) 0x000B8000;

 /** fb_write_cell:
     *  Writes a character with the given foreground and background to position i
     *  in the framebuffer.
     *
     *  @param i  The location in the framebuffer
     *  @param c  The character
     *  @param fg The foreground color
     *  @param bg The background color
     */
    void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
    {
        fb[2*i] = c;
        fb[2*i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
    }
    
    /* The I/O ports */
    #define FB_COMMAND_PORT         0x3D4
    #define FB_DATA_PORT            0x3D5

    /* The I/O port commands */
    #define FB_HIGH_BYTE_COMMAND    14
    #define FB_LOW_BYTE_COMMAND     15

    /** fb_move_cursor:
     *  Moves the cursor of the framebuffer to the given position
     *
     *  @param pos The new position of the cursor
     */
    void fb_move_cursor(unsigned short pos)
    {
        outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
        outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
        outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
        outb(FB_DATA_PORT,    pos & 0x00FF);
    }
    
    int fb_write(char *buf, unsigned int len);

    #define FB_GREEN     2
    #define FB_DARK_GREY 8
    #define FB_LIGHT_CYAN 3
    #define FB_BLACK      0
    
    int fb_write(char *buf, unsigned int len) {
  	unsigned int indexToBuffer = 0;
  	while (indexToBuffer < len) {
    	fb_write_cell(indexToBuffer, buf[indexToBuffer], FB_LIGHT_CYAN, FB_BLACK);
    	fb_move_cursor(indexToBuffer);
    	indexToBuffer++;
    }
    return 0;
}

/* Function to initialize */
void init() {
  /* Initialize segment descriptor tables */
  init_gdt();

  /* Initialize serial port */
  serial_configure(SERIAL_COM1_BASE, 1);
}

    
int kmain(int arg1, int arg2, int arg3)
{
    	char buffer[25] = "--- helloworld ---";
    	fb_write(buffer, 18);
    	serial_configure(SERIAL_COM1_BASE, 1);
    	serial_write(SERIAL_COM1_BASE, buffer, 18);
	//fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
	//fb_move_cursor(00);
	return arg1 + arg2 + arg3;
}
