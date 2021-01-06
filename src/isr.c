
#include "monitor.h"
#include "common.h"
#include "isr.h"


void isr_handler(registers_t reg) {

	monitor_write("interrupt recieved!",0,15);
	monitor_write_dec(reg.int_no);
	monitor_write("\n",0,15);

}