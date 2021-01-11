// monitor.c -- Defines functions for writing to the monitor.
//             heavily based on Bran's kernel development tutorials,
//             but rewritten for JamesM's kernel tutorials.

#include "monitor.h"

// The VGA framebuffer starts at 0xB8000.
u16int *video_memory = (u16int *)0xB8000;
// Stores the cursor position.
u8int cursor_x = 0;
u8int cursor_y = 0;

// Updates the hardware cursor.
static void move_cursor() {

    u16int cursor_location = cursor_y*80 + cursor_x;
    outb(0x3D4, 14);                     // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursor_location >> 8);   // send high byte of cursor_location

    outb(0x3D4, 15);                // tell the VGA board low byte is coming
    outb(0x3D5, cursor_location);   // send low byte of cursor_location

 }
        

// Scrolls the text on the screen up by one line.
static void scroll()
{

    // Get a space character with the default colour attributes.
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if(cursor_y >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }
        // The cursor should now be on the last line.
        cursor_y = 24;
    }
}

// Writes a single character out to the screen.
void monitor_put(char c, u8int bg_colour, u8int fg_colour)
{

    // 16-bit word format
   /*
    *   4 bit - background color
    *   4 bit - background color
    *   8 bit - specific character code 
    *
    *   0 - 7   ---> character code
    *   8 - 11  ---> foreground colour
    *   12 - 15 ---> background colour
    *
    */

    // The background colour is black (0), the foreground is white (15).
    u8int backColour = bg_colour;
    u8int foreColour = fg_colour;

    // The attribute byte is made up of two nibbles - the lower being the 
    // foreground colour, and the upper the background colour.
    u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    u16int attribute = attributeByte << 8;
    u16int *location;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && cursor_x)
    {
        location = video_memory + (cursor_y*80 + cursor_x)-1;
        *location = blank | attribute;
        cursor_x--;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        cursor_x = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y ++;
    }

    // Scroll the screen if needed.
    scroll();
    // Move the hardware cursor.
    move_cursor();

}

void printf(char *str,...){
    
    



    static u8int x = 0, y = 0;
    //uint8_t state;

    for(int i = 0; str[i] != '\0'; ++i) {

        switch(str[i]) {

            case '\n':
                x = 0;
                y++;
                break;
            default:
                video_memory[80*y + x]= (video_memory[i] & 0xFF00) | str[i];
                x++;
                break;
        }
        if( x >= 80){
            y++;
            x = 0;

        }
        if(y >= 25){
            for(y = 0; y <= 25; y++ ){
                for( x = 0; x<=80; x++){
                    video_memory[80*y+x] = (video_memory[i] & 0xFF00) | ' ';
                }
            }
            x = 0;
            y = 0;
        }
    }
    
    
}


// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
    // Make an attribute byte for the default colours
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++)
    {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c, u8int bg_colour, u8int fg_colour)
{
    int i = 0;
    while (c[i])
    {
        monitor_put(c[i++], bg_colour, fg_colour);
    }
}

void monitor_write_hex(u32int n)
{
    // TODO: implement this yourself!
    char num[32];
    int i = 0;

    while(n > 0) {
        int res = n%16;
        if(res > 9) {
            switch(res) {
                case 10:
                    num[i++] = 'A';
                    break;
                case 11:
                    num[i++] = 'B';
                    break;
                case 12:
                    num[i++] = 'C';
                    break;
                case 13:
                    num[i++] = 'D';
                    break;  
                case 14:
                    num[i++] = 'E';
                    break;
                case 15:
                    num[i++] = 'F';
                    break;
            }
        }
        else{ 
            num[i++] = '0' + res;
        }
        n = n/16;
    }

     // reverse the string
    int l = 0, r = i-1;
    while( r > l) {
        char tmp = num[l];
        num[l] = num[r];
        num[r] = tmp;
        l++; r--;
    }

    monitor_write("0x",0,15);
    monitor_write(num,0,15);
    
}

void monitor_write_dec(u32int n)
{
    // TODO: implement this yourself!

    char num[32];
    int i = 0;

    while(n > 0) {
        num[i++] = '0' + n%10;
        n = n/10;
    }

    // reverse the string
    int l = 0, r = i-1;
    while( r > l) {
        char tmp = num[l];
        num[l] = num[r];
        num[r] = tmp;
        l++; r--;
    } 

    monitor_write(num,0,7);
    
}
