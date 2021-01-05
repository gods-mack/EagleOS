
#include "common.h"


/* 
 *
 *
 *
 */ 

struct gdt_struct {

	u16int limit_low;

	u16int base_low;
	u8int base middle;
	u8int access;
	u8int granularity;
	u8int base_high;

} __attribute__((packed));

typedef gdt_struct gdt_entry_t;


// To tell the processor where to find our GDT, 
// we have to give it the address of a special pointer structure: 
// The base is the address of the first entry in our GDT, 
// the limit being the size of the table minus one (the last valid address in the table). 

struct gdt_pointer_struct {

	u16int low;  // The upper 16 bits of all selector limits.
	u32int base; // address of the first gdt_entry_t struct

} __attribute__((packed));

typedef gdt_pointer_struct gdt_ptr_t;

void init_descriptor_tables(); 



struct idt_entry_struct {



}__attribute__((packed));
