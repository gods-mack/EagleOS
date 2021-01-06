// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr) {
    // Initialise the screen (by clearing it)

	init_descriptor_tables();
  	monitor_clear();
    // Write out a sample string
    monitor_write("\n",0,0);
    monitor_write("                        Welcome to the my OS world!                             \n",10,0	);
    monitor_write("\nApplications",0,5);
    monitor_write("\n> Calculator",0,15);
    monitor_write("\n> Date/Time",0,15);
    monitor_write("\n> Greeting\n",0,15);

    asm volatile ("int $0x3");
	asm volatile ("int $0x4");
    //monitor_write_dec(2423);
  //  monitor_write("");
    

    
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
