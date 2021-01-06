// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

//#include "monitor.h"

int main(struct multiboot *mboot_ptr)
{
    // Initialise the screen (by clearing it)
  	monitor_clear();
    // Write out a sample string
    monitor_write("\n");
    monitor_write("                        Welcome to the my OS world!                             \n");
    monitor_write("\nApplications",0,5);
    monitor_write("\n> Calculator",0,15);
    monitor_write("\n> Data/Time",0,15);
    monitor_write("\n> Greeting",0,15);
    

    
 return 0;
}

/*

Value 	Color 	Value 	Color
0 	BLACK 	8 	DARK GREY
1 	BLUE 	9 	LIGHT BLUE
2 	GREEN 	10 	LIGHT GREEN
3 	CYAN 	11 	LIGHT CYAN
4 	RED 	12 	LIGHT RED
5 	MAGENTA 	13 	LIGHT MAGENTA
6 	BROWN 	14 	LIGHT BROWN
7 	LIGHT GREY 	15 	WHITE   

*/
