
#include "common.h"

// Initialisation function is publicly accessible.
void init_descriptor_tables();


/* 
 *
 *
 *
 */ 


struct gdt_struct {

	u16int limit_low;

	u16int base_low;
	u8int base_middle;
	u8int access;
	u8int granularity;
	u8int base_high;

} __attribute__((packed));

typedef struct gdt_struct gdt_entry_t;


// To tell the processor where to find our GDT, 
// we have to give it the address of a special pointer structure: 
// The base is the address of the first entry in our GDT, 
// the limit being the size of the table minus one (the last valid address in the table). 

struct gdt_pointer_struct {

	u16int limit;  // The upper 16 bits of all selector limits.
	u32int base; // address of the first gdt_entry_t struct

} __attribute__((packed));
typedef struct gdt_pointer_struct gdt_ptr_t;




// A struct describing an interrupt gate.
struct idt_struct {

   u16int base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   u16int sel;                 // Kernel segment selector.
   u8int  always0;             // This must always be zero.
   /*
   * flag byte format
   * 0-4 => always 00110
   * 5-6 => DPL
   * 7   => P
   */

   u8int  flags;               // More flags. See documentation.
   u16int base_hi;   		   // The upper 16 bits of the address to jump to.

} __attribute__((packed));
typedef struct idt_struct idt_entry_t;


// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_pointer_struct {
	
	u16int limit;
	u32int base;  // address of first idt_entry_t array

} __attribute__((packed));
typedef struct idt_pointer_struct idt_ptr_t;

	


// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


