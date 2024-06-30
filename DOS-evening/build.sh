#!/bin/bash
mkdir -p bin
gcc -m32 -ffreestanding -O0 -Wall -Wextra -c src/boot.s -o bin/boot.o 
gcc -I src/include -m32 -ffreestanding -O0 -Wall -Wextra -c src/kernel.c -o bin/kernel.o
gcc -I src/include -m32 -ffreestanding -O0 -Wall -Wextra -c src/hal.c -o bin/hal.o
gcc -I src/include -m32 -ffreestanding -O0 -Wall -Wextra -c src/kernlib.c -o bin/kernlib.o
gcc -I src/include -m32 -ffreestanding -O0 -Wall -Wextra -c src/video.c -o bin/video.o
gcc -m32 -ffreestanding -O0 -T src/link.ld -o isodir/myos.elf -nostdlib \
 bin/boot.o bin/kernel.o bin/hal.o bin/kernlib.o bin/video.o

if grub-file --is-x86-multiboot isodir/myos.elf; then
    echo Multiboot confirmed
else
    echo The file is not multiboot
fi

if grub-file --is-x86-multiboot2 isodir/myos.elf; then
    echo Multiboot2 confirmed
else
    echo The file is not multiboot2
fi

xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label isodir -o MyOS-limine.iso