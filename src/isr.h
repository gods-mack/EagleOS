
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