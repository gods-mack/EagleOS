sudo mount -w floppy.img /mnt/kernel
sudo cp src/kernel /mnt/kernel/kernel
sleep 1
sudo umount /mnt/kernel

qemu-system-i386 -fda  floppy.img -boot a
