
//
// descriptor_tables.c - Initialises the GDT and IDT, and defines the
// default ISR and IRQ handler.
// Based on code from Bran's kernel development tutorials.

#include "common.h"
#include "descriptor_tables.h"


// Lets us access our ASM functions from our C code.
extern void gdt_flush(u32int);  // gdt_flush() is the function that actually tells the processor where the new GDT exists,
extern void idt_flush(u32int);



static void init_gdt();
//static void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);


gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


void init_descriptor_table() {

	init_gdt();
}




/*  What should I put in my GDT?  

   --> The null descriptor which is never referenced by the 
       processor. Certain emulators, like Bochs, will complain 
       about limit exceptions if you do not have one present. Some use this
       descriptor to store a pointer to the GDT itself (to use with the LGDT instruction). 
       The null descriptor is 8 bytes wide and the pointer is 6 bytes 
       wide so it might just be the perfect place for this.
   --> A code segment descriptor (for your kernel, it should have type=0x9A)
   --> A data segment descriptor (you can't write to a code segment, so add this with type=0x92)
   --> A user mode code segment  descriptor (type=oxFA)
   --> A user mode data segment descriptor (type=oxF2)

   // miscellaneous 
   --> A TSS segment descriptor (trust me, keep a place for at least one)
       Room for more segments if you need them (e.g. user-level, LDTs, more TSS, whatever) 


*/

// gdt_init then sets up the 5 descriptors
// the bits that are changing are the type and DPL fields. DPL is the descriptor privilege level - 3 
// for user code and 0 for kernel code. Type specifies whether the segment is a code segment or a 
// data segment (the processor checks this often, and can be the source of much frustration). 
static void init_gdt() {

	gdt_ptr.limit = ((sizeof(gdt_entry_t) * 5) - 1);
	gdt_ptr.base  = (u32int)&gdt_entries;

// NULL segment
	gdt_entries[0].limit_low     = 0;
	gdt_entries[0].base_low      = 0;
	gdt_entries[0].base_middle   = 0;
	gdt_entries[0].access        = 0;
	gdt_entries[0].granularity   = 0;
	gdt_entries[0].base_high     = 0;

// CODE segment
   	gdt_entries[1].base_low    = (0 & 0xFFFF);
   	gdt_entries[1].base_middle = (0 >> 16) & 0xFF;
   	gdt_entries[1].base_high   = (0 >> 24) & 0xFF;

   	gdt_entries[1].limit_low   = (0xFFFFFFFF & 0xFFFF);
   	gdt_entries[1].granularity = (0xFFFFFFFF >> 16) & 0x0F;

   	gdt_entries[1].granularity |= 0xCF & 0xF0;
   	gdt_entries[1].access      = 0x9A;

// DATA segment
   	gdt_entries[2].base_low    = (0 & 0xFFFF);
   	gdt_entries[2].base_middle = (0 >> 16) & 0xFF;
   	gdt_entries[2].base_high   = (0 >> 24) & 0xFF;

   	gdt_entries[2].limit_low   = (0xFFFFFFFF & 0xFFFF);
   	gdt_entries[2].granularity = (0xFFFFFFFF >> 16) & 0x0F;

   	gdt_entries[2].granularity |= 0xCF & 0xF0;
   	gdt_entries[2].access      = 0x92;


// USER mode CODE segment
   	gdt_entries[3].base_low    = (0 & 0xFFFF);
   	gdt_entries[3].base_middle = (0 >> 16) & 0xFF;
   	gdt_entries[3].base_high   = (0 >> 24) & 0xFF;

   	gdt_entries[3].limit_low   = (0xFFFFFFFF & 0xFFFF);
   	gdt_entries[3].granularity = (0xFFFFFFFF >> 16) & 0x0F;

   	gdt_entries[3].granularity |= 0xCF & 0xF0;
   	gdt_entries[3].access      = 0xFA;   	


// USER mode DATA segment
   	gdt_entries[4].base_low    = (0 & 0xFFFF);
   	gdt_entries[4].base_middle = (0 >> 16) & 0xFF;
   	gdt_entries[4].base_high   = (0 >> 24) & 0xFF;

   	gdt_entries[4].limit_low   = (0xFFFFFFFF & 0xFFFF);
   	gdt_entries[4].granularity = (0xFFFFFFFF >> 16) & 0x0F;

   	gdt_entries[4].granularity |= 0xCF & 0xF0;
   	gdt_entries[4].access      = 0xF2;   	

   

        // gdt_flush() is the function that actually tells the processor where the new GDT exists,
   	gdt_flush((u32int)&gdt_ptr);

}


