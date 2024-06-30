#!/bin/bash

sudo apt install gcc xorriso grub-common git mtools qemu-system-x86

mkdir -p isodir
git clone https://github.com/limine-bootloader/limine.git --branch=v7.x-binary --depth=1
cd limine
make
cp limine-bios-cd.bin   ../isodir/
cp limine-uefi-cd.bin   ../isodir/
cp limine-bios.sys      ../isodir/
cp limine-bios-pxe.bin  ../isodir/
cd ..
