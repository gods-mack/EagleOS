// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "kbd.h"
#include "paging.h"
#include "fs.h"
#include "initrd.h"
#include "multiboot.h"

extern u32int placement_address;

int main(struct multiboot *mboot_ptr) {

    init_descriptor_tables();
    initialise_paging();
    
    monitor_clear();

    monitor_write("\n",0,0);
    monitor_write("                        Welcome to the EagleOS!                                 \n",10,0);
    monitor_write("\nApplications",0,5);
    monitor_write("\n> Calculator",0,15);
    monitor_write("\n> Date/Time",0,15);
    monitor_write("\n> Greeting\n",0,15);

      //int num = 10/0; // devode by zero exception

    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("int $0x0");




    //monitor_write_hex(763);
   __asm__ __volatile__ ("sti");  // allow IRQs to happen. 
  


    // Testing Virtual memory and Heap
    /*
    u32int a = kmalloc(8);
    //initialise_paging();
    u32int b = kmalloc(8);
    u32int c = kmalloc(8);
    monitor_write("\na: ",0,15);
    monitor_write_hex(a);
    monitor_write("\n, b: ",0,15);
    monitor_write_hex(b);
    monitor_write("\nc: ",0,15);
    monitor_write_hex(c);

    kfree(c);
    kfree(b);
    */


    // Testing VFS (vitual file system)

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);



    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        monitor_write("Found file ",0,15);
        monitor_write(node->name,0,15);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY)
            monitor_write("\n\t(directory)\n",0,15);
        else
        {
            monitor_write("\n\t contents: \"",0,15);
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j], 0, 15);

            monitor_write("\"\n",0,15);
        }
        i++;
    }


    init_timer(100);
    init_kbd();        // start keyboard driver
    
    int x = 782;
    u32int *ptr = (u32int*)&x;
    monitor_write_dec(*ptr);

    monitor_write("\n",0,15);
    monitor_write("\n",0,15);
    monitor_write("     ************************************************************    \n",0,15);
    monitor_write("\n",0,15);
    monitor_write("\n",0,15);
    //while(1) {
    monitor_write("root@manish:~$ ",0,2);
      

   // }

    

    return 0;
}

/*

Value   Color   Value   Color
0   BLACK          8    DARK GREY
1   BLUE             9  LIGHT BLUE
2   GREEN          10   LIGHT GREEN
3   CYAN             11     LIGHT CYAN
4   RED              12     LIGHT RED
5   MAGENTA          13     LIGHT MAGENTA
6   BROWN          14   LIGHT BROWN
7   LIGHT GREY     15   WHITE   

*/
