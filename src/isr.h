
//
// isr.h -- Interface and structures for high level interrupt service routines.
// Part of this code is modified from Bran's kernel development tutorials.
// Rewritten for JamesM's kernel development tutorials.
//

#include "common.h"

typedef struct registers {

	u32int ds;   // Data segment selector
	u32int edi, esi, edp, esp, ebx, edx, ecx, eax; // pushed by pusha
	u32int int_no, err_code;   // interrupt number and error code
	u32int eip, cs, eflags, useresp, ss; // pushed by processor automatically

} registers_t;


/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */
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