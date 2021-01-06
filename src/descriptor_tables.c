
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

static void init_idt();
static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags);



gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;


void init_descriptor_tables() {

	init_gdt();
  init_idt();
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
   /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    * 
    */
   	gdt_entries[1].base_low    = (0 & 0xFFFF);
   	gdt_entries[1].base_middle = (0 >> 16) & 0xFF;
   	gdt_entries[1].base_high   = (0 >> 24) & 0xFF;

   	gdt_entries[1].limit_low   = (0xFFFFFFFF & 0xFFFF);
   	gdt_entries[1].granularity = (0xFFFFFFFF >> 16) & 0x0F;

   	gdt_entries[1].granularity |= 0xCF & 0xF0;
   	gdt_entries[1].access      = 0x9A;

// DATA segment
    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment 
    */
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



/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
static void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (u32int)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate( 0, (u32int)isr0 , 0x08, 0x8E); // Divide by zero
    idt_set_gate( 1, (u32int)isr1 , 0x08, 0x8E); // Debug Exception
    idt_set_gate( 2, (u32int)isr2 , 0x08, 0x8E); // Non Maskable Interrupt Exception
    idt_set_gate( 3, (u32int)isr3 , 0x08, 0x8E);
    idt_set_gate( 4, (u32int)isr4 , 0x08, 0x8E);
    idt_set_gate( 5, (u32int)isr5 , 0x08, 0x8E); // Out of Bounds Exception
    idt_set_gate( 6, (u32int)isr6 , 0x08, 0x8E);
    idt_set_gate( 7, (u32int)isr7 , 0x08, 0x8E);
    idt_set_gate( 8, (u32int)isr8 , 0x08, 0x8E);
    idt_set_gate( 9, (u32int)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32int)isr14, 0x08, 0x8E); // Page Fault Exception
    idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);

    idt_flush((u32int)&idt_ptr);
}

static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;

    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags   = flags /* | 0x60 */;
}



