
#include "monitor.h"
#include "common.h"
#include "isr.h"

unsigned char *exception_messages[] = 
{
    "#0 Division by zero exception",
    "#1 Debug exception",
    "#2 Non Maskable Interrupt",
    "#3 Breakpoint Exception",
    "#4 Into detected overflow exception",
    "#5 out of bound exception",
    "#6 invalid opcode exception",
    "#7 No Coprocessor Exception",
    "#8 double flault exception",
    "#9 coprocessor segment overrun exception",
    "#10 bad TSS exception",
    "#11 segment not present exception",
    "#12 stack flault exception",
    "#13 general protection exception",
    "#14 Page Fault exception",
    "#15 unknown interrupt exception",
    "#16 coprocessor Fault exception",
    "#17 alignment check exception (486+)",
    "#18 machine check exception",

    "Reserved 19",
    "Reserved 20 ",
    "Reserved 21",
    "Reserved 22",
    "Reserved 23",
    "Reserved 24",
    "Reserved 25",
    "Reserved 26",
    "Reserved 27",
    "Reserved 28",
    "Reserved 28",
    "Reserved 30",
    "Reserved 31"
};

// isr_t is typedef to function pointer void (*ptr_name)(register_t);
isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler) {

	interrupt_handlers[n] = handler;
}


void isr_handler(registers_t reg) {

	monitor_write("interrupt recieved! ",0,15);
	monitor_write(exception_messages[(int)reg.int_no],0,4);
//	monitor_write_dec(reg.int_no);
	monitor_write("\n",0,15);

}



// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs) {

   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
//  monitor_write("irq handler called\n",0,15);
   if (regs.int_no >= 40) {
   	
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0) {

       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
} 
