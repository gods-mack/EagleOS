// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "kbd.h"

int main(struct multiboot *mboot_ptr) {

    init_descriptor_tables();
    
  	monitor_clear();

    monitor_write("\n",0,0);
    monitor_write("                        Welcome to the EagleOS!                                 \n",10,0);
    monitor_write("\nApplications",0,5);
    monitor_write("\n> Calculator",0,15);
    monitor_write("\n> Date/Time",0,15);
    monitor_write("\n> Greeting\n",0,15);

 	  //int num = 10/0; // devode by zero exception

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
	  asm volatile ("int $0x0");
    monitor_write_dec(2432);

    monitor_write_hex(763);
    __asm__ __volatile__ ("sti");  // allow IRQs to happen. 
  
    init_timer(100);
    init_kbd();

    monitor_write("\n",0,15);
    monitor_write("\n",0,15);
    monitor_write("     ************************************************************    \n",0,15);
    monitor_write("\n",0,15);
    monitor_write("\n",0,15);
    //while(1) {
        monitor_write("root@manish:~$ ",0,2);
        while(1)
          scanf();

   // }

    

    return 0;
}

/*

Value 	Color 	Value 	Color
0 	BLACK 	       8 	DARK GREY
1 	BLUE 	         9 	LIGHT BLUE
2 	GREEN 	       10 	LIGHT GREEN
3 	CYAN 	         11 	LIGHT CYAN
4 	RED 	         12 	LIGHT RED
5 	MAGENTA 	     13 	LIGHT MAGENTA
6 	BROWN 	       14 	LIGHT BROWN
7 	LIGHT GREY 	   15 	WHITE   

*/
