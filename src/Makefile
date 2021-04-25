SOURCES=boot.o main.o monitor.o common.o descriptor_tables.o isr.o interrupt.o timer.o kbd.o kheap.o paging.o gdt.o ordered_array.o fs.o initrd.o

CC=gcc 
CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32
LDFLAGS=-m elf_i386  -Tlink.ld
ASFLAGS=-felf32

all: $(SOURCES) link

clean:
	-rm *.o kernel

link:
	ld -m elf_i386 $(LDFLAGS) -o kernel $(SOURCES)

mykernel.bin: 	

.s.o:
	nasm $(ASFLAGS) $<