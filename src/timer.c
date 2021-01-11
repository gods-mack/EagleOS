

//#include "common.h"
#include "timer.h"
#include "isr.h"

#include "monitor.h"
//#include "descriptor_tables.h"


u32int timer_tick = 0;

static void timer_handler(registers_t regs) {

	timer_tick++;

	//monitor_write("timer interrupt ",0,15);
//	monitor_write_dec(timer_tick);
//	monitor_write("\n",0,15);
	//display a message on the screen */
    if (timer_tick % 18 == 0)
    {
       // monitor_write("One second has passed\n",0,4);
    }

}

void init_timer(u32int freq_hz) {

	register_interrupt_handler(IRQ0, &timer_handler);

	
	u32int divisor = 1193180/freq_hz; // calculate our divisor

	outb(0x43, 0x36);              // set our command byte 0x36
	outb(0x40, divisor & 0xFF);    // set low byte of divisor
	outb(0x40, divisor >> 8);      // set high byte of divisor
	
	 

   
}

